# Linux Port Design — Mozzie

**Date:** 2026-04-04
**Status:** Approved
**Target Repository:** wj4616/mozzie (fork of usemozzie/mozzie)

## Summary

Add Linux (Debian/Ubuntu) support to Mozzie with .deb package builds. No application code changes required — only CI/CD configuration and documentation updates.

## Scope

- Target: Debian-based Linux distributions (Ubuntu, Linux Mint, Debian)
- Architecture: x86_64 only
- Format: .deb package
- Distribution: GitHub Releases (manual upload after CI artifact build)

## Changes

### 1. Tauri Configuration

**File:** `apps/desktop/src-tauri/tauri.conf.json`

Add `linux` bundle configuration:

```json
{
  "bundle": {
    "linux": {
      "deb": {
        "depends": [
          "webkit2gtk-4.1-0",
          "libgtk-3-0",
          "libssl3"
        ]
      }
    }
  }
}
```

### 2. Release Workflow

**File:** `.github/workflows/release.yml`

Add `build-linux` job after `build-macos`:

```yaml
  build-linux:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: pnpm/action-setup@v4
      - uses: actions/setup-node@v4
        with:
          node-version: 20
          cache: pnpm
      - uses: dtolnay/rust-toolchain@stable
      - uses: Swatinem/rust-cache@v2
        with:
          workspaces: apps/desktop/src-tauri

      - name: Install Tauri Linux dependencies
        run: |
          sudo apt-get update
          sudo apt-get install -y \
            libwebkit2gtk-4.1-dev \
            libgtk-3-dev \
            libayatana-appindicator3-dev \
            librsvg2-dev \
            file

      - name: Install dependencies
        run: pnpm install --frozen-lockfile

      - name: Build Tauri app
        working-directory: apps/desktop
        run: pnpm tauri build --bundles deb

      - name: Upload Linux artifacts
        uses: actions/upload-artifact@v4
        with:
          name: linux-x86_64
          path: apps/desktop/src-tauri/target/release/bundle/deb/*.deb
```

Update `release` job dependencies:

```yaml
  release:
    needs: [build-windows, build-macos, build-linux]
```

### 3. Build Test Workflow

**File:** `.github/workflows/build-test.yml`

Add `ubuntu-latest` to matrix:

```yaml
    strategy:
      matrix:
        include:
          - platform: windows-latest
          - platform: macos-latest
          - platform: ubuntu-latest
```

Add conditional dependency installation step (after checkout, before install):

```yaml
      - name: Install Tauri Linux dependencies
        if: matrix.platform == 'ubuntu-latest'
        run: |
          sudo apt-get update
          sudo apt-get install -y \
            libwebkit2gtk-4.1-dev \
            libgtk-3-dev \
            libayatana-appindicator3-dev \
            librsvg2-dev \
            file
```

### 4. README.md

Update Platforms section:

```markdown
## Platforms

- **macOS** — primary target, signed DMG builds
- **Windows** — supported
- **Linux** — Debian/Ubuntu (.deb), x86_64
```

### 5. CONTRIBUTING.md

Add Linux section after existing platform prerequisites:

```markdown
### Linux (Debian/Ubuntu)

Install system dependencies:

```bash
sudo apt update
sudo apt install -y \
  libwebkit2gtk-4.1-dev \
  libgtk-3-dev \
  libayatana-appindicator3-dev \
  librsvg2-dev \
  file
```

Then follow the standard build steps.
```

## Technical Notes

### Why No Code Changes?

The existing Rust code already handles Linux correctly:

```rust
// agents.rs and orchestrator.rs use this pattern:
let mut command = if cfg!(target_os = "windows") {
    let mut cmd = Command::new("cmd");
    cmd.arg("/C").arg(command_line);
    cmd
} else {
    // Unix-like systems (macOS and Linux)
    let mut cmd = Command::new(program);
    cmd.args(args);
    cmd
};
```

### Dependencies

**Runtime (.deb depends):**
- `webkit2gtk-4.1-0` — WebKitGTK webview renderer
- `libgtk-3-0` — GTK3 GUI libraries
- `libssl3` — TLS for LLM API calls

**Build-time (CI):**
- `libwebkit2gtk-4.1-dev` — Development headers
- `libgtk-3-dev` — GTK development files
- `libayatana-appindicator3-dev` — System tray support
- `librsvg2-dev` — SVG icon handling
- `file` — File-type detection

### External Tools

Mozzie depends on external tools that are available on Linux:
- `git` — Worktree operations (already in code via `std::process::Command`)
- `rg` (ripgrep) — File search (already in code)

Both are installable via apt on Debian/Ubuntu.

## Testing

### Local Testing (Linux)

```bash
# Install dependencies
sudo apt update
sudo apt install -y \
  libwebkit2gtk-4.1-dev \
  libgtk-3-dev \
  libayatana-appindicator3-dev \
  librsvg2-dev \
  file

# Build
pnpm install
pnpm tauri build --bundles deb

# Verify output
ls -la apps/desktop/src-tauri/target/release/bundle/deb/
```

### CI Testing

Push to branch → `build-test.yml` runs on `ubuntu-latest` → verify build passes.

## Out of Scope

- AppImage or .rpm builds
- ARM64 (aarch64) builds
- Linux-specific code features
- Automatic Notarization or signing (not required for Linux)
- apt repository publishing