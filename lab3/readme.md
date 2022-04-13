Anna Sztanga,

Mikalai Stelmakh

## SKPS22L, Laboratorium 3

### SDK
1. `wget https://downloads.openwrt.org/releases/21.02.1/targets/bcm27xx/bcm2711/openwrt-sdk-21.02.1-bcm27xx-bcm2711_gcc-8.4.0_musl.Linux-x86_64.tar.xz`
2. `tar -xaf openwrt-sdk-21.02.1-bcm27xx-bcm2711_gcc-8.4.0_musl.Linux-x86_64.tar.xz`
3. `make menuconfig`
    - W `Global Build Settings` wyłączamy
        - Select all target specific packages by default
        - Select all kernel module packages by default
        - Select all userspace packages by default
        - Cryptographically sign package lists
    - W `Advanced configuration options` wyłączamy `Automatic removal of build directories`
### 1. Pierwszy pakiet
1. `cd ..`
2. `https://moodle.usos.pw.edu.pl/pluginfile.php/217384/mod_folder/content/0/WZ_W03_przyklady.tar.xz` - pobranie pakietu
3. `tar -xaf WZ_W03_przyklady.tar.xz`
4. `cd openwrt-sdk`
5. `export LANG=C`
6. `nano feeds.conf.default`
    ```
    src-link skps /home/user/skps22_sztanga_stelmakh/lab3/demo1_owrt_pkg
    ```
7.
    ```
    ./scripts/feeds update -a
    ./scripts/feeds install -p skps -a
    ```
8. `make menuconfig`
    - W `Examples` zaznaczamy
        - demo1
        - demo1mak
9. 
    ```
    make package/feeds/skps/demo1/compile
    ```
10. `cd bin/packages/aarch64_cortex-a72/skps`
11. 
    ```
    scp -P 8888 demo1_1.0-1_aarch64_cortex-a72.ipk "root@localhost:~"
    ```
12. Instalujemy pakiety na OpenWRT:
    ```
    opkg install demo1_1.0-1_aarch64_cortex-a72.ipk
    ```
