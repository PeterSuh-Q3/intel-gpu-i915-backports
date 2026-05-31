#!/bin/bash
# patch_config.sh
# defconfigs/drm 항목을 .config 에 강제 적용

CONFIG_FILE=".config"

DEFCONFIG_FILES=(
    "defconfigs/drm"
)

# ────────────────────────────────────────────
apply_defconfig() {
    local defconfig_file="$1"

    if [[ ! -f "$defconfig_file" ]]; then
        echo "[WARN] $defconfig_file 없음 - 건너뜀"
        return
    fi

    echo "  ┌─ Applying: $defconfig_file"
    while IFS= read -r line; do
        [[ "$line" =~ ^[[:space:]]*# ]] && continue
        [[ -z "$line" ]] && continue
        key=$(echo "$line" | grep -oP '^CPTCFG_\w+')
        [[ -z "$key" ]] && continue
        desired_value="$line"

        if grep -qE "^${key}=" "$CONFIG_FILE" 2>/dev/null; then
            current=$(grep -E "^${key}=" "$CONFIG_FILE")
            if [[ "$current" != "$desired_value" ]]; then
                sed -i "s|^${key}=.*|${desired_value}|" "$CONFIG_FILE"
                echo "  │  [REPLACED] $current  →  $desired_value"
            else
                echo "  │  [OK]       $desired_value"
            fi
        elif grep -qE "^# ${key} is not set" "$CONFIG_FILE" 2>/dev/null; then
            sed -i "s|^# ${key} is not set|${desired_value}|" "$CONFIG_FILE"
            echo "  │  [ENABLED]  # $key is not set  →  $desired_value"
        else
            echo "$desired_value" >> "$CONFIG_FILE"
            echo "  │  [ADDED]    $desired_value"
        fi
    done < "$defconfig_file"
    echo "  └──────────────────────────────────────"
}

verify_status() {
    local defconfig_file="$1"
    local has_error=0

    [[ ! -f "$defconfig_file" ]] && return 0

    echo "  ┌─ Verify: $defconfig_file"
    while IFS= read -r line; do
        [[ "$line" =~ ^[[:space:]]*# ]] && continue
        [[ -z "$line" ]] && continue
        key=$(echo "$line" | grep -oP '^CPTCFG_\w+')
        [[ -z "$key" ]] && continue
        result=$(grep -E "^${key}=|^# ${key} is not set" "$CONFIG_FILE" 2>/dev/null)
        if [[ -z "$result" ]]; then
            echo "  │  [ERROR]   $key → 여전히 없음!"
            has_error=1
        else
            echo "  │  [CHECK]   $result"
        fi
    done < "$defconfig_file"
    echo "  └──────────────────────────────────────"
    return $has_error
}

# ══════════════════════════════════════════════
if [[ ! -f "$CONFIG_FILE" ]]; then
    echo "[ERROR] .config 없음 - make defconfig-drm 을 먼저 실행하세요."
    exit 1
fi

echo "========================================"
echo " [STEP] Patching .config ..."
echo "========================================"
for f in "${DEFCONFIG_FILES[@]}"; do
    apply_defconfig "$f"
done

echo ""
echo "========================================"
echo " [STEP] Verify"
echo "========================================"
overall_error=0
for f in "${DEFCONFIG_FILES[@]}"; do
    verify_status "$f"
    [[ $? -ne 0 ]] && overall_error=1
done

echo ""
if [[ $overall_error -eq 0 ]]; then
    echo "[SUCCESS] 모든 항목 적용 완료"
else
    echo "[FAILURE] 일부 항목 적용 실패"
    exit 1
fi
