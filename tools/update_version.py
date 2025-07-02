#!/bin/python
#
# maddy update version
#
# Replaces the version in all files in the lines containing
# `MADDY_VERSION_LINE_REPLACEMENT`.
#
# This project is licensed under the MIT license. For more information see the
# LICENSE file.

import mimetypes
import os
import re
from semver import Version
import sys

def update_version_in_file(version, file_path):
  """
  Updates the version in the specified file.

  Args:
      file_path (str): The path to the file where the version needs to be updated.
      version (str): The new version string to set in the file.
  """
  with open(file_path, 'r') as file:
    content = file.read()

  new_content = content
  lines = new_content.splitlines()

  for i, line in enumerate(lines):
    if 'MADDY_VERSION_LINE_REPLACEMENT' in line:
      lines[i] = re.sub(r'\b\d+\.\d+\.\d+\b', version, line)

  new_content = '\n'.join(lines) + '\n'

  if new_content != content:
    print(f'Updating version in {file_path}')
    with open(file_path, 'w', encoding='utf-8', newline='\n') as file:
      file.write(new_content.replace('\r\n', '\n'))


def update_version_in_files(version, directory, ignored_dirs):
  """
  Updates the version in all files in the specified directory.

  Args:
      directory (str): The path to the directory containing files.
      ignored_dirs (list): A list of directories to ignore during the update.
      version (str): The new version string to set in the files.
  """
  mimetypes.add_type('text/markdown', '.md')

  ignroed_dirs_abs = [os.path.abspath(os.path.join(directory, d)) for d in ignored_dirs]

  for root, dirs, files in os.walk(directory):
    for file in files:
      file_path = os.path.join(root, file)

      if any(os.path.abspath(root).startswith(ignored_dir) for ignored_dir in ignroed_dirs_abs):
        continue

      if mimetypes.guess_type(file_path)[0] and mimetypes.guess_type(file_path)[0].startswith('text'):
        update_version_in_file(version, file_path)


def main():
  if len(sys.argv) < 3:
    print('Usage: python update_version.py <version> <directory> [ignored_dirs...]')
    sys.exit(1)

  version = sys.argv[1]
  directory = sys.argv[2]
  ignored_dirs = sys.argv[3:] if len(sys.argv) > 3 else []

  if not os.path.isdir(directory):
    print(f'Error: The specified directory "{directory}" does not exist.')
    sys.exit(1)

  if not Version.is_valid(version):
    print(f'Error: The specified version "{version}" is not a valid semantic version.')
    sys.exit(1)

  update_version_in_files(version, directory, ignored_dirs)
  print(f'Updated version to {version} in all files in {directory}')


if __name__ == '__main__':
    main()
