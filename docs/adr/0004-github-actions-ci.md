# 4. Use GitHub Actions to build and test every pull request in the dev container

> Draft: the reasons below come from the repository contents, not from the team's board. Replace or confirm them before accepting.

## Context

The project is meant to receive outside contributions later. Every change must be checked the same
way, without relying on the author having run the tests. The check should use the same environment
as our own machines, so that "it builds for me" and "it builds in CI" mean the same thing (see
ADR 1). It runs the same commands as `make test` (see ADR 2 and ADR 3).

The repository is hosted on GitHub, which provides GitHub Actions.

Alternatives:

- No CI: contributors and reviewers run the tests by hand.
- Another CI service (GitLab CI, CircleCI, Jenkins).
- GitHub Actions with the `devcontainers/ci` action, which reads `devcontainer.json` directly but
  adds a third-party dependency.

## Decision

We use GitHub Actions. The workflow `.github/workflows/ci.yml` runs on every pull request and on
every push to `main`. It builds the image from `.devcontainer/Dockerfile` with `docker build`,
then runs `make test` (the `debug` preset) inside it with `docker run`.

It uses the `pull_request` trigger, so pull requests from forks run without access to any secret,
and the workflow only has read access to the repository.

## Status

Proposed

## Consequences

- Every pull request is built and tested in the same environment as a local dev container.
- Only GitHub's own `actions/checkout` is used. There is no third-party action to keep up to date.
- The CI depends on GitHub, and moving to another service means rewriting the workflow. The
  commands themselves (`docker build`, `docker run`, `make test`) would stay the same.
- The image is rebuilt on every run and there is no caching, so each run takes longer than it
  could. Caching can be added later.
- Only the `debug` preset is checked. The `release` and `sanitize` presets are not run in CI.
- CI runs `make test`, so the `Makefile` is now part of what CI relies on.
- CI runs the `Dockerfile`, not `devcontainer.json`, so the editor integration (VS Code, Zed) is
  not tested. Native builds on Windows and macOS are not tested either.
- A failing check only blocks a merge if the repository is configured to require it. That setting
  is not part of this change.
- Usage limits and pricing of GitHub Actions were not checked.
