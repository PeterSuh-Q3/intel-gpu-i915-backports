# Intel® Graphics Driver Backports for Linux® OS (intel-gpu-i915-backports)

Contains the backported kernel module source code of intel GPUs on [Proxmox VE](https://pve.proxmox.com/) and DSM for SA6400. You can create Dynamic Kernel Module Support (DKMS) as well as precompiled Out of Tree modules packages, which can be installed on supported OS distributions.

We are using [backport project](https://backports.wiki.kernel.org/index.php/Main_Page) to generate out of tree i915 kernel module source codes.

This repo is a code snapshot of https://github.com/intel/mainline-tracking/tree/linux/v6.5 and does not contain individual git change history.


## Out of tree kernel drivers
This repository contains following drivers.
1. Intel® Graphics Driver Backports(i915) - The main graphics driver (includes a compatible DRM subsystem and dmabuf if necessary)
2. AMD GPU Driver (amdgpu) - Optional, built on the `feature/amdgpu-merge` branch. The AMD graphics driver merged into the same backport tree so that it links against the **same** DRM core as i915 (see below).


## AMD GPU (amdgpu) dual-DRM merge

On the `feature/amdgpu-merge` branch this repository additionally backports the
**AMD `amdgpu` driver from Linux v6.5** into the same out-of-tree tree as i915.

### Why
When i915 and amdgpu are built as separate out-of-tree packages, each one ships
its own `drm.ko`. Those two `drm.ko` files are **ABI-incompatible** (different
size and dmabuf dependencies), so they cannot coexist on a single system. By
compiling both drivers from one tree against one DRM core, a **single shared
`drm.ko`** is produced and the ABI conflict disappears.

### Design
- The `amdgpu` source is taken from **Linux v6.5**, matching the v6.5 DRM core
  this backport is based on. An older (e.g. 5.10) amdgpu would clash with the
  6.5 DRM API (`drm_device.pdev`, removed gem callbacks, `ring_mirror_list` →
  `pending_list`, etc.).
- AMD/amdgpu-specific Kconfig symbols are renamed `CONFIG_*` → `CPTCFG_*`,
  consistent with the backport namespace.
- The DRM GPU scheduler (`drivers/gpu/drm/scheduler`, producing `gpu-sched.ko`)
  is added as well: stock i915-backports does not ship it because i915 has its
  own scheduler, but amdgpu requires the exported `drm_sched_*` symbols.
- The default configuration is **headless** (`DRM_AMD_DC=n`, `HSA_AMD=n`,
  `DRM_AMDGPU_SI=n`, `DRM_AMDGPU_CIK=n`) — suitable for compute / VA-API
  transcode on DSM.

### Host-kernel (5.10) compatibility
Mainline v6.5 amdgpu calls kernel APIs newer than the DSM 5.10.55 host. These
are bridged with small `backport-include/linux/*.h` shims and `#ifdef BPM_*`
source guards (the same marker mechanism i915 already uses), e.g.
`cc_platform_has`, `vga_client_register`/`unregister`, `kmap_local_page`,
`cancel_work`, `apple_gmux_detect`, `PCI_CLASS_ACCELERATOR_PROCESSING`,
`DECLARE_FLEX_ARRAY`, `generic_handle_domain_irq`,
`DEFINE_DEBUGFS_ATTRIBUTE_SIGNED`, and `pci_driver.dev_groups`.

### Build output
A successful build yields i915, amdgpu (plus `amdxcp`, `gpu-sched`) and the
shared DRM helper stack (`drm`, `ttm`, `drm_buddy`, `drm_ttm_helper`,
`drm_suballoc_helper`, `drm_kms_helper`, `drm_display_helper`, `dmabuf`,
`intel-gtt`, …) — all resolving against one `drm.ko` with no unresolved
symbols. See `CLAUDE.md` for the full design notes and the iteration log.


## Dependencies

  These drivers have dependency on Intel® GPU firmware and few more kernel mode drivers may be needed based on specific use cases, platform, and distributions. Source code of additional drivers should be available at https://github.com/intel-gpu

- [Intel® GPU firmware](https://git.kernel.org/pub/scm/linux/kernel/git/firmware/linux-firmware.git/tree/i915) - Firmware required by intel GPUs.

Each project is tagged consistently, so when pulling these repos, pull the same tag.

## Supported OS Kernel/Distribution
  Our current backport supports the following OS Distribution.

| OS Distribution | OS Version | Kernel Version  | Installation Instructions |
|---  |---  |---  |--- |
| Proxmox VE | 8.1--8.2 | 6.5, 6.8 | Install the released .deb via `apt install` |
| DSM for SA6400 | 7.1.x / 7.2.x | 5.10.55 | Shipped by supported loaders |


## Product Releases:
Please refer [Releases](https://dgpu-docs.intel.com/releases/index.html)
