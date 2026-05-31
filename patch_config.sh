#!/bin/bash
# patch_config.sh
# defconfigs/drm_i915 의 항목을 .config 에 강제 적용
# - 값이 다르면 덮어쓰기
# - "# CPTCFG_xxx is not set" 형태면 제거 후 추가
# - 아예 없는 항목이면 맨 끝에 추가

CONFIG_FILE=".config"
DEFCONFIG_FILE="defconfigs/drm_i915"

echo "========================================"
echo " Before: 대상 항목 현재 상태"
echo "========================================"

# defconfigs에서 CPTCFG_ 항목명만 추출하여 grep 검사
while IFS= read -r line; do
    # 주석이나 빈 줄 제외, CPTCFG_ 항목만
    [[ "$line" =~ ^[[:space:]]*# ]] && continue
    [[ -z "$line" ]] && continue

    key=$(echo "$line" | grep -oP '^CPTCFG_\w+')
    [[ -z "$key" ]] && continue

    result=$(grep -E "^${key}=|^# ${key} is not set" "$CONFIG_FILE")
    if [[ -z "$result" ]]; then
        echo "  [MISSING] $key  →  항목 없음"
    else
        echo "  [FOUND]   $result"
    fi
done < "$DEFCONFIG_FILE"

echo ""
echo "========================================"
echo " Patching .config ..."
echo "========================================"

while IFS= read -r line; do
    [[ "$line" =~ ^[[:space:]]*# ]] && continue
    [[ -z "$line" ]] && continue

    key=$(echo "$line" | grep -oP '^CPTCFG_\w+')
    [[ -z "$key" ]] && continue

    desired_value="$line"   # e.g. CPTCFG_DRM=m

    if grep -qE "^${key}=" "$CONFIG_FILE"; then
        # 값이 다르면 sed로 교체
        current=$(grep -E "^${key}=" "$CONFIG_FILE")
        if [[ "$current" != "$desired_value" ]]; then
            sed -i "s|^${key}=.*|${desired_value}|" "$CONFIG_FILE"
            echo "  [REPLACED] $current  →  $desired_value"
        else
            echo "  [OK]       $desired_value"
        fi
    elif grep -qE "^# ${key} is not set" "$CONFIG_FILE"; then
        # not set 행을 제거하고 원하는 값으로 교체
        sed -i "s|^# ${key} is not set|${desired_value}|" "$CONFIG_FILE"
        echo "  [ENABLED]  # $key is not set  →  $desired_value"
    else
        # 아예 없으면 파일 끝에 추가
        echo "$desired_value" >> "$CONFIG_FILE"
        echo "  [ADDED]    $desired_value  (새로 추가됨)"
    fi
done < "$DEFCONFIG_FILE"

echo ""
echo "========================================"
echo " After: 적용 결과 확인"
echo "========================================"

while IFS= read -r line; do
    [[ "$line" =~ ^[[:space:]]*# ]] && continue
    [[ -z "$line" ]] && continue

    key=$(echo "$line" | grep -oP '^CPTCFG_\w+')
    [[ -z "$key" ]] && continue

    result=$(grep -E "^${key}=|^# ${key} is not set" "$CONFIG_FILE")
    if [[ -z "$result" ]]; then
        echo "  [ERROR]   $key  →  여전히 없음!"
    else
        echo "  [CHECK]   $result"
    fi
done < "$DEFCONFIG_FILE"

echo ""
echo "Done."
