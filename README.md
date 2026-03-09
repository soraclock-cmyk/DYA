COROPITを使用する方は以下のようにコードを編集してください。

mona2_r.overlay

修正前
```
  trackball_central: trackball_central@0 {
        status = "okay";
        compatible = "pixart,pmw3610";  //トラボセンサ用のドライバとバインド
        reg = <0>;
        spi-max-frequency = <2000000>;
        irq-gpios = <&gpio0 2 (GPIO_ACTIVE_LOW | GPIO_PULL_UP)>; //P0.02を指定(MOTION)
        cpi = <600>;
        //swap-xy;
        //invert-x; //COROPIT版ではコメントアウトを外す
        //invert-y; //COROPIT版ではコメントアウトを外す
        evt-type = <INPUT_EV_REL>;
        x-input-code = <INPUT_REL_X>;
        y-input-code = <INPUT_REL_Y>;
    };
};

```
**修正後**
```
  trackball_central: trackball_central@0 {
        status = "okay";
        compatible = "pixart,pmw3610";  //トラボセンサ用のドライバとバインド
        reg = <0>;
        spi-max-frequency = <2000000>;
        irq-gpios = <&gpio0 2 (GPIO_ACTIVE_LOW | GPIO_PULL_UP)>; //P0.02を指定(MOTION)
        cpi = <600>;
        //swap-xy;
        invert-x; //COROPIT版ではコメントアウトを外す
        invert-y; //COROPIT版ではコメントアウトを外す
        evt-type = <INPUT_EV_REL>;
        x-input-code = <INPUT_REL_X>;
        y-input-code = <INPUT_REL_Y>;
    };
};

```

## DYA Studio対応版のビルド前提（重要）

このリポジトリは ZMK + Zephyr でビルドするため、**ローカルで `west build` する場合は Zephyr SDK が必要**です。

- エラー例: `Could not find a package configuration file provided by "Zephyr-sdk"`
- これはコード不備ではなく、ローカル開発環境に SDK/ツールチェーンが無い時に発生します。

### 1) 推奨: ZMK公式の開発コンテナを使う

ZMK公式 dev container / GitHub Actions には必要なツールチェーンが含まれるため、
通常は追加設定なしでビルドできます。

### 2) ローカルに直接入れる場合

1. `west` をインストール
2. ワークスペース初期化
3. `west update`
4. Zephyr SDK をインストール
5. `west zephyr-export`
6. `west build`

例:

```bash
python -m pip install west
west init -l config
west update --narrow
west zephyr-export
west build -s zmk/app -b seeeduino_xiao_ble -- -DZMK_CONFIG=$PWD/config -DSHIELD="mona2_r rgbled_adapter" -DSNIPPET="studio-rpc-usb-uart"
```

### 3) このリポジトリでの狙い

- 右手トラックボール: `pmw3610` + runtime input processor
- 左手エンコーダー: runtime sensor rotate
- DYA Studio RPC関連モジュール有効化

上記は設定済みのため、**残る失敗要因は主にローカル環境（Zephyr SDK未導入）**です。
