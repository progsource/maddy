#!/bin/python
#
# maddy update dependencies
# This project is licensed under the MIT license. For more information see the
# LICENSE file.
import os
import re
import requests
import sys

def get_cmake_files(directory, ignored_dirs=None):
  """
  Recursively searches for all CMakeLists.txt files in the specified directory,
  ignoring specified subdirectories.

  Args:
      directory (str): The path to the directory to search for CMakeLists.txt files.
      ignored_dirs (list): A list of directory names to ignore during the search.

  Returns:
      list: A list of full paths to all found CMakeLists.txt files.
  """
  cmakelists_paths = []
  ignored_dirs = ignored_dirs or []

  for root, dirs, files in os.walk(directory):
    # Modify dirs in place to skip ignored directories
    dirs[:] = [d for d in dirs if d not in ignored_dirs]

    if "CMakeLists.txt" in files:
      cmakelists_paths.append(os.path.join(root, "CMakeLists.txt"))

  return cmakelists_paths

def get_last_dependency_version(url):
  """
  Retrieves the latest release version tag from a specified GitHub repository.

  Args:
      url (str): The URL of the GitHub repository in the format
                  'https://github.com/owner/repo.git' or 'https://github.com/owner/repo'.

  Returns:
      str: The latest release version tag (e.g., 'v1.2.3') if found,
            or raises an Exception if the repository is not found or
            if there is an error in the API request.

  Raises:
      ValueError: If the provided URL is not in the expected format.
      Exception: If there is an error fetching data from the GitHub API.
  """
  # Remove the .git suffix if it exists
  if url.endswith('.git'):
    url = url[:-4]

  # Split the URL to extract owner and repository name
  parts = url.split('/')
  if len(parts) < 5 or parts[2] != 'github.com':
    raise ValueError(f"Invalid GitHub repository URL {url}")

  owner = parts[3]
  repo = parts[4]

  # GitHub API endpoint for releases
  api_url = f"https://api.github.com/repos/{owner}/{repo}/releases/latest"

  # Make a GET request to the GitHub API
  response = requests.get(api_url)

  if response.status_code == 200:
    release_data = response.json()
    return release_data['tag_name']  # Return the latest version tag
  else:
    raise Exception(f"Error fetching data from GitHub API: {response.status_code} - {response.text}")

def get_current_version_from_fetch_content(cmake_code):
  """
  Extracts the currently used Git tag from a FetchContent call in CMake code.

  Args:
      cmake_code (str): The CMake code containing the FetchContent call.

  Returns:
      str: The Git tag used in the FetchContent call, or None if not found.
  """
  # Regular expression to match FetchContent_Declare and capture the GIT_TAG argument
  pattern = r'FetchContent_Declare\s*\(\s*[^)]+\s*GIT_TAG\s+([^\s)]+)'

  match = re.search(pattern, cmake_code)
  if match:
    return match.group(1)  # Return the captured Git tag
  return None

def update_fetch_content_versions(cmake_file_path):
  """
  Updates the FetchContent blocks in a CMakeLists.txt file to use the latest
  version from the corresponding GitHub repositories.

  Args:
      cmake_file_path (str): The path to the CMakeLists.txt file to be updated.
  """
  with open(cmake_file_path, 'r') as file:
    cmake_code = file.read()

  # Regular expression to find FetchContent blocks
  fetch_content_pattern = r'FetchContent_Declare\s*\(\s*(.*?)\s*\)'

  # Find all FetchContent blocks
  fetch_content_blocks = re.findall(fetch_content_pattern, cmake_code, re.DOTALL)

  for block in fetch_content_blocks:
    # Extract the GIT_REPOSITORY line
    repo_pattern = r'GIT_REPOSITORY\s+([^\s]+)'
    repo_match = re.search(repo_pattern, block)

    if repo_match:
      repo_url = repo_match.group(1)
      current_version = get_current_version_from_fetch_content(block)
      latest_version = get_last_dependency_version(repo_url)

      if current_version != latest_version:
        # Replace the old version with the new version in the CMake code
        new_block = re.sub(r'GIT_TAG\s+([^\s]+)', f'GIT_TAG {latest_version}', block)
        cmake_code = cmake_code.replace(block, new_block)

  # Write the updated CMake code back to the file
  with open(cmake_file_path, 'w', encoding='utf-8', newline='\n') as file:
    file.write(cmake_code.replace('\r\n', '\n'))  # Ensure LF line

def main():
  """
  Main function to update FetchContent versions in CMakeLists.txt files.
  It takes a directory path as an argument and processes all CMakeLists.txt files found,
  ignoring specified directories.
  """
  if len(sys.argv) < 2:
    print("Usage: python update_dependencies.py <directory_path> [ignored_dirs...]")
    sys.exit(1)

  directory_path = sys.argv[1]
  ignored_dirs = sys.argv[2:]  # Remaining arguments are ignored directories

  if not os.path.isdir(directory_path):
    print(f"The provided path '{directory_path}' is not a valid directory.")
    sys.exit(1)

  cmake_files = get_cmake_files(directory_path, ignored_dirs)

  for cmake_file in cmake_files:
    print(f"Updating {cmake_file}...")
    update_fetch_content_versions(cmake_file)
    print(f"Updated {cmake_file} successfully.")

if __name__ == "__main__":
    main()
