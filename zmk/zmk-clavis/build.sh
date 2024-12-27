#!/bin/bash

west build \
    -b "nice_nano_v2" \
    -d /build/settings_reset \
    -s /zmk/zmk/app \
    -- -DSHIELD="settings_reset" -DZMK_CONFIG="/zmk/config"


#ls -alh /build/settings_reset/zephyr/zmk.uf2

# west build \
#     -d /build/left -p \
#     -b "nice_nano_v2" \
#     -s /zmk/zmk/app \
#     -- -DSHIELD="corne_left nice_view_adapter nice_view" -DZMK_CONFIG="/zmk/config" 

#west build -d build/right -b nice_nano_v2 -- -DSHIELD=corne_right -DZMK_CONFIG='../../samoklava-zmk-config/config'

for side in right left; do
    west build -b "nice_nano_v2" \
        -d /build/${side} \
        -s /zmk/zmk/app \
        -- -DSHIELD="corne_${side}" -DZMK_CONFIG="/zmk/config"
done
#west build -b "nice_nano_v2" -d /build/right -p -s /zmk/zmk/app -- -DSHIELD="corne_right" -DZMK_CONFIG="/zmk/config"
#-- ${{ steps.main.outputs.shield_arg }} ${{ steps.main.outputs.modules_arg }} 
    
#-DZMK_CONFIG="${GITHUB_WORKSPACE}/miryoku_zmk/config"

#\ -DZMK_EXTRA_MODULES="/zmk-urchin/urchin-zmk-module"

#west build -d /build/right -p -b "nice_nano_v2" \ -s /zmk-urchin/zmk/app \ -- -DSHIELD="urchin_right nice_view_adapter nice_view" \ -DZMK_CONFIG="/zmk-urchin/config" \ -DZMK_EXTRA_MODULES="/zmk-urchin/urchin-zmk-module"

cp /build/left/zephyr/zmk.uf2 /firmware/clavis_left.uf2
cp /build/right/zephyr/zmk.uf2 /firmware/clavis_right.uf2
cp /build/settings_reset/zephyr/zmk.uf2 /firmware/settings_reset.uf2
