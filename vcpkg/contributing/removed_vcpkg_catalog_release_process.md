---
title: Microsoft/vcpkg Catalog Release Process
description: Internal process guide for creating a new curated catalog release of vcpkg.
ms.date: 11/30/2022
ROBOTS: NOINDEX
---

# Microsoft/vcpkg catalog release process

This document describes the acceptance criteria / process we use when doing a vcpkg release.

1. Generate a new GitHub Personal Access Token with repo permissions.
1. Using the PAT, invoke $/scripts/Get-Changelog.ps1 `-StartDate (previous release date) -EndDate (Get-Date) -OutFile path/to/results.md`
1. Create a new GitHub release in this repo.
1. Submit a vcpkg.ci (full tree rebuild) run with the same SHA as that release.
1. Use the "auto-generate release notes". Copy the "new contributors" and "full changelog" parts to the end of `path/to/results.md`.
1. Change `## New Contributors` to `#### New Contributors`
1. In `path/to/results.md`, update `LINK TO BUILD` with the most recent link to vcpkg.ci run.
1. In `path/to/results.md`, fill out the tables for number of existing ports and successful ports.
1. Replace the contents of the release notes with the contents of `path/to/results.md`
1. After the full rebuild submission completes, update the link to the one for the exact SHA, the
   counts, and remove "(tentative)".
