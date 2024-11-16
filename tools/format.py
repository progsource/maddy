import glob
import os
import shlex
import subprocess
import sys

# required clang-format version
REQUIRED_VERSION = "18.1.3"

def check_clang_format_version():
  """
  Check if the installed clang-format version matches the required version.

  Raises:
      SystemExit: If the version does not match the required version or if
                  there is an error checking the version.
  """
  try:
    result = subprocess.run(
      ['clang-format', '--version'],
      stdout=subprocess.PIPE,
      stderr=subprocess.PIPE,
      text=True,
      check=True
    )
    version_out = result.stdout.strip().split()

    version_line = '0.0.0'
    is_next = False

    for vo in version_out:
      if vo == 'version':
        is_next = True
        continue
      if is_next:
        version_line = vo
        break

    if version_line != REQUIRED_VERSION:
      print(f"Error: Required clang-format version is "
        f"{REQUIRED_VERSION}, but found {version_line}.")
      sys.exit(1)
    else:
      print('clang-format version equals the required version.')
  except subprocess.CalledProcessError as e:
    print(f"Error checking clang-format version: {e.stderr}")
    sys.exit(1)

def format_files(dry_run):
  """
  Format .h and .cpp files in specified directories using clang-format.

  Args:
      dry_run (bool): If True, performs a dry run to check for formatting
                      issues without modifying files. If False, formats
                      the files in place.

  Raises:
      subprocess.CalledProcessError: If there is an error running clang-format
                                     during the actual formatting process.
  """
  patterns = [
    "include/**/*.h",
    "tests/**/*.h",
    "tests/**/*.cpp",
  ]

  files_to_format = []
  for pattern in patterns:
    matched_files = glob.glob(pattern, recursive=True)
    files_to_format.extend(matched_files)

  if not files_to_format:
    print("No files to format.")
    return

  # Create a space-separated string of files
  patterns_arg = ' '.join(file.replace('\\', '/') for file in files_to_format)

  cwd = os.getcwd()

  if dry_run:
    # Check for changes without modifying the files
    command = f'clang-format --style=file --dry-run --Werror {patterns_arg}'
    result = subprocess.run(
      shlex.split(command),
      stdout=subprocess.PIPE,
      stderr=subprocess.PIPE,
      text=True,
      cwd=cwd
    )
    if result.returncode != 0:
      print("Files that need formatting:")
      print(result.stdout)
      print("Error output:")
      print(result.stderr)
      sys.exit(1)
    else:
      print("no changes found")
  else:
    # Format the files in place
    command = f'clang-format --style=file -i {patterns_arg}'
    result = subprocess.run(
      shlex.split(command),
      stdout=subprocess.PIPE,
      stderr=subprocess.PIPE,
      text=True,
      cwd=cwd
    )
    if result.returncode != 0:
      print("Error formatting files:")
      print(result.stderr)
      sys.exit(1)
    else:
      print('formatting done')

def main():
  """
  Main function to handle command-line arguments and invoke formatting.

  Checks for the required command-line argument, verifies the clang-format
  version, and calls the format_files function to format the code.
  """
  if len(sys.argv) != 2:
    print(
      "Usage: python check_clang_format.py <dry_run|format>"
    )
    sys.exit(1)
  else:
    print(f"Running format with {sys.argv[1]}")

  dry_run = False
  if sys.argv[1] == "dry_run":
    dry_run = True
  elif sys.argv[1] != "format":
    print("Invalid argument. Use 'dry_run' or 'format'.")
    sys.exit(1)

  check_clang_format_version()
  format_files(dry_run)

if __name__ == "__main__":
  main()
