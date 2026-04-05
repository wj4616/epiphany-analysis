# Linux Port Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add Debian/Ubuntu Linux build support to Mozzie via CI/CD configuration and Tauri bundle settings.

**Architecture:** Additive configuration changes only — no Rust or TypeScript code modifications. Tauri 2.0's webkit2gtk backend already works on Linux; we just need to enable CI builds and document dependencies.

**Tech Stack:** GitHub Actions, Tauri 2.0, webkit2gtk-4.1, apt packages

---

## Files Changed

| File | Action |
|------|--------|
| `apps/desktop/src-tauri/tauri.conf.json` | Modify — add `linux` bundle config |
| `.github/workflows/release.yml` | Modify — add `build-linux` job |
| `.github/workflows/build-test.yml` | Modify — add `ubuntu-latest` to matrix |
| `README.md` | Modify — add Platforms section |
| `CONTRIBUTING.md` | Modify — add Linux prerequisites section |

---

### Task 1: Add Linux Bundle Configuration to Tauri

**Files:**
- Modify: `apps/desktop/src-tauri/tauri.conf.json`

- [ ] **Step 1: Read current tauri.conf.json**

Run: `cat apps/desktop/src-tauri/tauri.conf.json`

Find the `bundle` object. It currently has `active`, `targets`, `icon`, and `macOS` keys.

- [ ] **Step 2: Add linux section to bundle config**

Add `"linux"` key after `"macOS"` in the bundle object:

```json
    "macOS": {
      "entitlements": null,
      "signingIdentity": null,
      "providerShortName": null,
      "minimumSystemVersion": "10.15"
    },
    "linux": {
      "deb": {
        "depends": [
          "webkit2gtk-4.1-0",
          "libgtk-3-0",
          "libssl3"
        ]
      }
    }
  },
```

- [ ] **Step 3: Verify JSON is valid**

Run: `cat apps/desktop/src-tauri/tauri.conf.json | python3 -m json.tool > /dev/null && echo "Valid JSON"`

Expected: `Valid JSON`

- [ ] **Step 4: Commit**

```bash
git add apps/desktop/src-tauri/tauri.conf.json
git commit -m "feat(linux): add linux deb bundle configuration"
```

---

### Task 2: Add Linux Build Job to Release Workflow

**Files:**
- Modify: `.github/workflows/release.yml`

- [ ] **Step 1: Read current release.yml**

Run: `cat .github/workflows/release.yml`

Locate the `build-macos` job (ends around line 225) and the `release` job (starts around line 227).

- [ ] **Step 2: Add build-linux job after build-macos**

Insert this new job after the closing of `build-macos` job (after line 225, before `release` job):

```yaml
  # ─── Linux ───────────────────────────────────────────────────────
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

  # ─── Create GitHub Release ────────────────────────────────────────
```

- [ ] **Step 3: Update release job dependencies**

Find the `release` job's `needs` array (around line 228). Change from:

```yaml
  release:
    needs: [build-windows, build-macos]
```

To:

```yaml
  release:
    needs: [build-windows, build-macos, build-linux]
```

- [ ] **Step 4: Validate YAML**

Run: `python3 -c "import yaml; yaml.safe_load(open('.github/workflows/release.yml'))" && echo "Valid YAML"`

Expected: `Valid YAML`

- [ ] **Step 5: Commit**

```bash
git add .github/workflows/release.yml
git commit -m "feat(ci): add linux build job to release workflow"
```

---

### Task 3: Add Linux to Build Test Workflow Matrix

**Files:**
- Modify: `.github/workflows/build-test.yml`

- [ ] **Step 1: Read current build-test.yml**

Run: `cat .github/workflows/build-test.yml`

Find the matrix strategy section (around line 16-21).

- [ ] **Step 2: Add ubuntu-latest to matrix**

Add ubuntu-latest to the `include` array:

```yaml
    strategy:
      fail-fast: false
      matrix:
        include:
          - platform: windows-latest
          - platform: macos-latest
          - platform: ubuntu-latest
```

- [ ] **Step 3: Add Linux dependency installation step**

After the `uses: dtolnay/rust-toolchain@stable` step and before `name: Install dependencies`, add:

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

Note: Place this after step 4 (`dtolnay/rust-toolchain@stable`) in the current file.

- [ ] **Step 4: Validate YAML**

Run: `python3 -c "import yaml; yaml.safe_load(open('.github/workflows/build-test.yml'))" && echo "Valid YAML"`

Expected: `Valid YAML`

- [ ] **Step 5: Commit**

```bash
git add .github/workflows/build-test.yml
git commit -m "feat(ci): add ubuntu-latest to build-test matrix"
```

---

### Task 4: Add Platforms Section to README

**Files:**
- Modify: `README.md`

- [ ] **Step 1: Read current README**

Run: `cat README.md`

Note: The current README does **not** have a Platforms section. It mentions platforms informally in other sections.

- [ ] **Step 2: Add Platforms section after Architecture section**

After the "Agents communicate via ACP..." line (line 57) and before "## Contributing" (line 59), insert:

```markdown
## Platforms

- **macOS** — primary target, signed DMG builds (Apple Silicon and Intel)
- **Windows** — supported, NSIS installer
- **Linux** — Debian/Ubuntu (.deb), x86_64

```

- [ ] **Step 3: Verify README renders correctly**

Run: `head -70 README.md`

Expected: Platforms section visible between Architecture and Contributing sections.

- [ ] **Step 4: Commit**

```bash
git add README.md
git commit -m "docs: add Platforms section to README"
```

---

### Task 5: Add Linux Prerequisites to CONTRIBUTING

**Files:**
- Modify: `CONTRIBUTING.md`

- [ ] **Step 1: Read current CONTRIBUTING.md**

Run: `cat CONTRIBUTING.md`

Find where platform-specific prerequisites would go. The "Prerequisites" section (lines 5-10) links to Tauri docs. Add a new section after "Getting Started" (ends at line 24) and before "Project Structure" (line 28).

- [ ] **Step 2: Add Platform-Specific Setup section after Getting Started**

After line 26 (blank line after "pnpm dev" section), insert:

```markdown
## Platform-Specific Setup

### Linux (Debian/Ubuntu)

Install system dependencies before running `pnpm dev`:

```bash
sudo apt update
sudo apt install -y \
  libwebkit2gtk-4.1-dev \
  libgtk-3-dev \
  libayatana-appindicator3-dev \
  librsvg2-dev \
  file
```

### macOS

No additional setup required. Xcode Command Line Tools and Homebrew are sufficient for Tauri prerequisites.

### Windows

No additional setup required. Microsoft Visual Studio Build Tools are sufficient for Tauri prerequisites.

```

- [ ] **Step 3: Verify file is valid**

Run: `cat CONTRIBUTING.md`

Expected: New section visible between "Getting Started" and "Project Structure".

- [ ] **Step 4: Commit**

```bash
git add CONTRIBUTING.md
git commit -m "docs: add platform-specific setup to CONTRIBUTING"
```

---

### Task 6: Final Verification and Push

- [ ] **Step 1: Review all changes**

Run: `git log --oneline -5`

Expected: 5 commits for Tasks 1-5.

- [ ] **Step 2: Verify file changes**

Run: `git diff HEAD~5 --stat`

Expected: 5 files modified.

- [ ] **Step 3: Create feature branch and push (optional)**

If working on a fork:

```bash
git checkout -b feat/linux-support
git push origin feat/linux-support
```

---

## Testing

### Local Verification (Linux only)

If running on Linux Mint/Debian/Ubuntu:

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

### CI Verification

After pushing to GitHub:

1. Check `.github/workflows/build-test.yml` runs on `ubuntu-latest`
2. Verify Linux job passes in Actions tab

---

## Rollback

If issues arise:

```bash
git revert HEAD~5..HEAD  # Revert all 5 commits
git push origin main --force-with-lease
```