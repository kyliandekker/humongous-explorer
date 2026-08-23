#!/usr/bin/env python3

from pathlib import Path
import argparse
import re


# ============================================================
# Configuration
# ============================================================

SOURCE_EXTENSIONS = {
    ".c",
    ".cc",
    ".cpp",
    ".cxx",
    ".h",
    ".hh",
    ".hpp",
    ".hxx",
}


INCLUDE_RE = re.compile(
    r'^(\s*#\s*include\s+)(<[^>]+>|"[^"]+")(.*)$'
)


# ============================================================
# File discovery
# ============================================================

def find_source_files(root: Path):
    """Recursively find every C/C++ source file."""

    if root.is_file():
        if root.suffix.lower() in SOURCE_EXTENSIONS:
            yield root
        return

    for path in root.rglob("*"):
        if (
            path.is_file()
            and path.suffix.lower() in SOURCE_EXTENSIONS
        ):
            yield path


# ============================================================
# Source header detection
# ============================================================

def get_source_stem(path: Path):
    """Get the source file name without its extension."""

    source_extensions = {
        ".c",
        ".cc",
        ".cpp",
        ".cxx",
    }

    if path.suffix.lower() in source_extensions:
        return path.stem

    return None


def is_own_header(
    include: str,
    source_stem: str | None,
):
    """
    Determine whether an include is the source file's
    corresponding header.
    """

    if source_stem is None:
        return False

    if not include.startswith('"'):
        return False

    include_path = include.strip('"')

    include_path = include_path.replace(
        "\\",
        "/",
    )

    while include_path.startswith("./"):
        include_path = include_path[2:]

    return (
        Path(include_path).stem.lower()
        == source_stem.lower()
    )


# ============================================================
# Namespace detection
# ============================================================

def get_namespace(include: str):
    """
    Get the first directory component.

    Examples:

        "core/Memory.h"
            -> core

        "imgui/Helpers.h"
            -> imgui

        "imgui/views/SearchBar.h"
            -> imgui

        "resources/Resource.h"
            -> resources

        "Foo.h"
            -> None
    """

    if not include.startswith('"'):
        return None

    include_path = include.strip('"')

    include_path = include_path.replace(
        "\\",
        "/",
    )

    while include_path.startswith("./"):
        include_path = include_path[2:]

    parts = include_path.split("/")

    if len(parts) < 2:
        return None

    return parts[0]


# ============================================================
# Include normalization
# ============================================================

def normalize_project_include(
    include: str,
):
    """
    Normalize project includes so they always use their
    full project path.
    """

    if not include.startswith('"'):
        return include

    include_path = include.strip('"')

    include_path = include_path.replace(
        "\\",
        "/",
    )

    while include_path.startswith("./"):
        include_path = include_path[2:]

    return f'"{include_path}"'


def normalize_own_header(
    include: str,
    source_stem: str,
):
    """
    Normalize the source's own header.

    Always produces:

        "./Header.h"
    """

    if not include.startswith('"'):
        return include

    original_path = include.strip('"')

    original_path = original_path.replace(
        "\\",
        "/",
    )

    extension = Path(
        original_path
    ).suffix

    if not extension:
        extension = ".h"

    return (
        f'"./{source_stem}{extension}"'
    )


# ============================================================
# Sort include section
# ============================================================

def sort_includes(
    lines: list[str],
    file_path: Path,
):
    """
    Sort a complete include section.

    Result:

        #include "./OwnHeader.h"

        // external
        #include <algorithm>
        #include <string>

        // project
        #include "core/Memory.h"

        #include "imgui/Helpers.h"
        #include "imgui/ImGuiSystem.h"

        #include "resources/Resource.h"
        #include "resources/ResourceFactory.h"

    """

    source_stem = get_source_stem(
        file_path
    )

    own_headers = []
    external = []
    root_project = []

    # namespace -> includes
    namespaces = {}

    # ========================================================
    # Collect every include first
    # ========================================================

    for line in lines:

        match = INCLUDE_RE.match(line)

        if not match:
            continue

        prefix = match.group(1)
        include = match.group(2)
        suffix = match.group(3)

        # ----------------------------------------------------
        # Own header
        # ----------------------------------------------------

        if is_own_header(
            include,
            source_stem,
        ):

            normalized = normalize_own_header(
                include,
                source_stem,
            )

            formatted = (
                f"{prefix}"
                f"{normalized}"
                f"{suffix}"
            )

            own_headers.append(
                (
                    normalized.lower(),
                    formatted,
                )
            )

            continue

        # ----------------------------------------------------
        # External includes
        # ----------------------------------------------------

        if include.startswith("<"):

            formatted = (
                f"{prefix}"
                f"{include}"
                f"{suffix}"
            )

            external.append(
                (
                    include.lower(),
                    formatted,
                )
            )

            continue

        # ----------------------------------------------------
        # Project includes
        # ----------------------------------------------------

        normalized = normalize_project_include(
            include
        )

        formatted = (
            f"{prefix}"
            f"{normalized}"
            f"{suffix}"
        )

        namespace = get_namespace(
            normalized
        )

        # ----------------------------------------------------
        # Project include without namespace
        # ----------------------------------------------------

        if namespace is None:

            root_project.append(
                (
                    normalized.lower(),
                    formatted,
                )
            )

        # ----------------------------------------------------
        # Namespaced project include
        # ----------------------------------------------------

        else:

            namespace_key = namespace.lower()

            if namespace_key not in namespaces:
                namespaces[namespace_key] = []

            namespaces[namespace_key].append(
                (
                    normalized.lower(),
                    formatted,
                )
            )

    # ========================================================
    # Sort groups
    # ========================================================

    own_headers.sort(
        key=lambda item: item[0]
    )

    external.sort(
        key=lambda item: item[0]
    )

    root_project.sort(
        key=lambda item: item[0]
    )

    for namespace in namespaces:
        namespaces[namespace].sort(
            key=lambda item: item[0]
        )

    # ========================================================
    # Build final output
    # ========================================================

    result = []

    # ========================================================
    # Own header
    # ========================================================

    if own_headers:

        for _, line in own_headers:
            result.append(line)

    # ========================================================
    # External
    # ========================================================

    if external:

        if result:
            result.append("")

        result.append("// external")

        for _, line in external:
            result.append(line)

    # ========================================================
    # Project
    # ========================================================

    has_project = (
        root_project
        or namespaces
    )

    if has_project:

        if result:
            result.append("")

        result.append("// project")

    # --------------------------------------------------------
    # Root project includes
    # --------------------------------------------------------

    if root_project:

        for _, line in root_project:
            result.append(line)

    # --------------------------------------------------------
    # Namespaces
    # --------------------------------------------------------

    for namespace in sorted(namespaces):

        if root_project or namespace != sorted(namespaces)[0]:
            result.append("")

        for _, line in namespaces[namespace]:
            result.append(line)

    # ========================================================
    # Blank line after include section
    # ========================================================

    if result:
        result.append("")

    return result


# ============================================================
# Sort one file
# ============================================================

def sort_file(
    file_path: Path,
    write: bool = True,
):
    """
    Sort includes in one file.

    Blank lines between includes are considered part of the
    same include section.
    """

    try:

        text = file_path.read_text(
            encoding="utf-8",
            errors="surrogateescape",
        )

    except PermissionError:

        print(
            f"SKIPPED (permission denied): "
            f"{file_path}"
        )

        return False

    except UnicodeDecodeError:

        print(
            f"SKIPPED (not UTF-8): "
            f"{file_path}"
        )

        return False

    final_newline = text.endswith("\n")

    lines = text.splitlines()

    output = []

    i = 0

    # ========================================================
    # Walk through file
    # ========================================================

    while i < len(lines):

        line = lines[i]

        # ====================================================
        # Start of include section
        # ====================================================

        if INCLUDE_RE.match(line):

            include_section = []

            while i < len(lines):

                current = lines[i]

                # --------------------------------------------
                # Include
                # --------------------------------------------

                if INCLUDE_RE.match(current):

                    include_section.append(current)

                    i += 1

                    continue

                # --------------------------------------------
                # Blank line between includes
                # --------------------------------------------

                if current.strip() == "":

                    j = i + 1

                    while (
                        j < len(lines)
                        and lines[j].strip() == ""
                    ):
                        j += 1

                    if (
                        j < len(lines)
                        and INCLUDE_RE.match(lines[j])
                    ):

                        i += 1

                        continue

                    break

                # --------------------------------------------
                # Normal code
                # --------------------------------------------

                break

            # ------------------------------------------------
            # Replace entire include section
            # ------------------------------------------------

            output.extend(
                sort_includes(
                    include_section,
                    file_path,
                )
            )

            # ------------------------------------------------
            # Remove original blank lines after the include
            # block. We generate our own.
            # ------------------------------------------------

            while (
                i < len(lines)
                and lines[i].strip() == ""
            ):
                i += 1

            continue

        # ====================================================
        # Normal line
        # ====================================================

        output.append(line)

        i += 1

    # ========================================================
    # Rebuild
    # ========================================================

    new_text = "\n".join(output)

    if final_newline:
        new_text += "\n"

    # ========================================================
    # No changes
    # ========================================================

    if new_text == text:

        print(
            f"OK:      {file_path}"
        )

        return False

    # ========================================================
    # Check mode
    # ========================================================

    if not write:

        print(
            f"Would sort: {file_path}"
        )

        return True

    # ========================================================
    # Write
    # ========================================================

    try:

        file_path.write_text(
            new_text,
            encoding="utf-8",
            errors="surrogateescape",
        )

    except PermissionError:

        print(
            f"SKIPPED (cannot write): "
            f"{file_path}"
        )

        return False

    print(
        f"Sorted:  {file_path}"
    )

    return True


# ============================================================
# Main
# ============================================================

def main():

    parser = argparse.ArgumentParser(
        description=(
            "Recursively sort and group "
            "C/C++ #include directives."
        )
    )

    parser.add_argument(
        "path",
        type=Path,
        help=(
            "File or directory to process."
        ),
    )

    parser.add_argument(
        "--check",
        action="store_true",
        help=(
            "Check without modifying files. "
            "Returns exit code 1 if changes are needed."
        ),
    )

    args = parser.parse_args()

    root = args.path

    # ========================================================
    # Validate
    # ========================================================

    if not root.exists():

        print(
            f"ERROR: path does not exist: "
            f"{root}"
        )

        raise SystemExit(1)

    # ========================================================
    # Find files
    # ========================================================

    files = sorted(
        find_source_files(root),
        key=lambda path: str(path).lower(),
    )

    print(
        f"Found {len(files)} C/C++ files."
    )

    print()

    # ========================================================
    # Process
    # ========================================================

    changed = 0

    for file_path in files:

        if sort_file(
            file_path,
            write=not args.check,
        ):
            changed += 1

    # ========================================================
    # Summary
    # ========================================================

    print()
    print("=" * 50)

    print(
        f"Processed: {len(files)} files"
    )

    print(
        f"Changed:   {changed} files"
    )

    # ========================================================
    # Check exit code
    # ========================================================

    if args.check and changed:
        raise SystemExit(1)


# ============================================================
# Entry point
# ============================================================

if __name__ == "__main__":
    main()