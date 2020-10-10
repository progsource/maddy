# Changelog

This file tries to follow roughly [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).
maddy uses [semver versioning](https://semver.org/).

## Badges

* ![**FIXED**](https://img.shields.io/badge/-FIXED-%23090) for any bug fixes.
* ![**SECURITY**](https://img.shields.io/badge/-SECURITY-%23c00) in case of vulnerabilities.
* ![**ADDED**](https://img.shields.io/badge/-ADDED-%23099) for new features.
* ![**CHANGED**](https://img.shields.io/badge/-CHANGED-%23e90) for changes in existing functionality.
* ![**DEPRECATED**](https://img.shields.io/badge/-DEPRECATED-%23666) for soon-to-be removed features.
* ![**REMOVED**](https://img.shields.io/badge/-REMOVED-%23900) for now removed features.

## Upcoming

* ![**ADDED**](https://img.shields.io/badge/-ADDED-%23099) Added Changelog
* ![**ADDED**](https://img.shields.io/badge/-ADDED-%23099) Added contribution guideline
* ?

## version 1.1.2 2020-10-04

* ![**ADDED**](https://img.shields.io/badge/-ADDED-%23090) `*`, `+` and `-` are equivalent for making unordered bullet list
* ![**ADDED**](https://img.shields.io/badge/-ADDED-%23090) Parsing support for fully numeric ordered lists
* ![**CHANGED**](https://img.shields.io/badge/-CHANGED-%23e90) make `Parser::Parse` accept istreams instead of stringstream
* ![**CHANGED**](https://img.shields.io/badge/-CHANGED-%23e90) CMake is creating an interface library which you can include in your own `target_link_libraries` and the global include path is untouched from maddy.

## version 1.1.1 2019-12-27

* ![**ADDED**](https://img.shields.io/badge/-ADDED-%23090) BreakLineParser
* ![**ADDED**](https://img.shields.io/badge/-ADDED-%23090) HTMLParser
* ![**ADDED**](https://img.shields.io/badge/-ADDED-%23090) Added optional config with the following options:
    * en-/disable the emphasized parser
    * wrap/not wrap HTML in markdown within a paragraph in output
* ![**CHANGED**](https://img.shields.io/badge/-CHANGED-%23e90) Updated gtest to release-1.10.0 to fix build issues


## version 1.1.0 2019-02-19

* ![**FIXED**](https://img.shields.io/badge/-FIXED-%23900) Added missing includes to BlockParser
* ![**FIXED**](https://img.shields.io/badge/-FIXED-%23900) Added missing dtor to BlockParser and LineParser
* ![**ADDED**](https://img.shields.io/badge/-ADDED-%23090) `__test__` can also be used to get `<strong>text</strong>`
* ![**ADDED**](https://img.shields.io/badge/-ADDED-%23090) Added AppVeyor CI
* ![**ADDED**](https://img.shields.io/badge/-ADDED-%23090) Added clang for CI
* ![**CHANGED**](https://img.shields.io/badge/-CHANGED-%23e90) Single underscore `_` results in emphasized tag `<em>`, single `*` in italic tag `<i>`

## version 1.0.3 2018-01-18

* ![**FIXED**](https://img.shields.io/badge/-FIXED-%23900) Make sure that all parsers are finished
* ![**FIXED**](https://img.shields.io/badge/-FIXED-%23900) ol documentation
* ![**ADDED**](https://img.shields.io/badge/-ADDED-%23090) Added Travic-CI with gcc
* ![**ADDED**](https://img.shields.io/badge/-ADDED-%23090) Added Howto for running the tests on the README

## version 1.0.2 2017-12-26

* ![**FIXED**](https://img.shields.io/badge/-FIXED-%23900) Fixed inline code for directly following letters (bold, emphasized and strikethrough)

## version 1.0.1 2017-12-25

* ![**FIXED**](https://img.shields.io/badge/-FIXED-%23900) Fixed inline code for bold, emphasized and strikethrough
* ![**FIXED**](https://img.shields.io/badge/-FIXED-%23900) Fixed spelling in README
* ![**ADDED**](https://img.shields.io/badge/-ADDED-%23090) Use Gold Linker on Unix if available for faster compile time
* ![**ADDED**](https://img.shields.io/badge/-ADDED-%23090) Added Github ISSUE_TEMPLATE

## version 1.0.0 2017-12-25

initial release
