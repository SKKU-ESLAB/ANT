# 연구개발성과 성능지표 평가 절차

## 1. 경량 JavaScript 런타임의 메모리 요구량

### 기존 경량 JavaSciprt 런타임 (IoT.js) 빌드
```bash
git clone https://github.com/jerryscript-project/iotjs.git
cd iotjs
./tools/build.py --clean --target-board=rpi2 --target-arch=arm
export IOTJS=${PWD}/build/arm-linux/debug/bin/iotjs
cd ..
```

### ANT 경량 JavaSciprt 런타임 (IoT.js-Plus) 빌드
```bash
git clone https://github.com/SKKU-ESLAB/IoTJS-Plus.git
cd IoTJS-Plus
./tools/build.py --clean --target-board=rpi2 --target-arch=arm
export IOTJS-Plus=${PWD}/build/arm-linux/debug/bin/iotjs
cd ..
```

### 벤치마크를 위한 JavaScript Application 다운로드
```bash
git clone https://github.com/SKKU-ESLAB/ANT.git
export APPBENCH_DIR=${PWD}/ANT/test/appbench/
```

```appbench``` 디렉토리에는 다음과 같은 JavaScript 벤치마크 앱이 포함되어 있습니다:
* ```v8-deltablue.js```: V8 benchmark's deltablue
* ```v8-raytrace.js```: V8 benchmark's raytrace
* ```sp-access-binary-trees.js```: SunSpider benchmark's access-binary-trees
* ```sp-crypto-sha1.js```: SunSpider benchmark's crypto-sha1

### 메모리 측정을 위한 메모리 프로파일러 빌드
```bash
git clone https://github.com/RedCarrottt/vminfo.git
cd vminfo
make
export VMINFO=${PWD}/vminfo
```

### 벤치마크를 실행 및 PSS 측정정
```bash
# Baseline (IoT.js)
${VMINFO} "${IOTJS} ${APPBENCH_DIR}/v8-deltablue.js" baseline-v8-deltablue.log -i
${VMINFO} "${IOTJS} ${APPBENCH_DIR}/v8-raytrace.js" baseline-v8-raytrace.log -i
${VMINFO} "${IOTJS} ${APPBENCH_DIR}/sp-access-binary-trees.js" baseline-sp-access-binary-trees.log -i
${VMINFO} "${IOTJS} ${APPBENCH_DIR}/sp-crypto-sha1.js" baseline-sp-crypto-sha1.log -i

# ANT (IoT.js-Plus)
${VMINFO} "${IOTJS-Plus} ${APPBENCH_DIR}/v8-deltablue.js" ant-v8-deltablue.log -i
${VMINFO} "${IOTJS-Plus} ${APPBENCH_DIR}/v8-raytrace.js" ant-v8-raytrace.log -i
${VMINFO} "${IOTJS-Plus} ${APPBENCH_DIR}/sp-access-binary-trees.js" ant-sp-access-binary-trees.log -i
${VMINFO} "${IOTJS-Plus} ${APPBENCH_DIR}/sp-crypto-sha1.js" ant-sp-crypto-sha1.log -i
```
생성된 로그 파일의 맨 마지막 값인 PSS (Proportional Set Size) 값을 확인하여 메모리 사용량을 비교합니다. 단위는 KB입니다.


## 2. 응용 프로그램 실행 시간

### ANT 프레임워크 설치 및 실행
```bash
git clone https://github.com/SKKU-ESLAB/ANT.git
cd ANT
./scripts/prepare-ubuntu.sh
./scripts/build-ant.sh
./out/antd
```

### Resource API vs. Legacy API 성능 측정
```bash
export RESOURCEBENCH_DIR${PWD}/test/resourcebench/

# Legacy API 수행
${IOTJS-Plus} ${RESOURCEBENCH_DIR}/legacy-api.js

# Resource API 수행
${IOTJS-Plus} ${RESOURCEBENCH_DIR}/resource-api.js
```
두 API의 latency를 측정하여 Legacy API 대비 ANT Resource API의 오버헤드를 정량적으로 평가합니다.


## 3. 머신러닝 실행 시간

### [Sever] 모델 압축 수행
```bash
git clone https://github.com/SKKU-ESLAB/Auto-Compression.git
cd Auto-Compression/pruning/UVP/ADMM_baseline
python3 main.py /data/imagenet -a mobilenet_v1 -j 16 --pretrained \
                --admm-epochs 100 --ft-epochs 100 -b 512 --lr 0.01 \
                --dataset imagenet --width-mult 1.0 --sparsity-method  uniform \
                --target-sparsity 0.9 --lr-scheduler cosine --rho 0.001 \
                --vector-size 4 --unaligned --cp --repeat
```
학습 완료 후, 최종 모델의 Top-1 정확도를 확인하여 기준치(정확도 손실 3% 이내)와 비교합니다.

### [Server] TFLite 포맷으로 모델 변환
```bash
python3 convert.py -a mobilenet_v1 --width-mult 1.0 \
                   --pretrained-model output/mobilenet_v1_1.0_ts0.9_admm100_ft100_v4_u_cp_repeat/model_best.pth.tar
```
변환된 모델은 ```mlbench/mbv1_0.9_v4u.tflite```로 업로드 하였습니다다.
 

### ML Benchmark 준비
본 평가에서는 공정성을 위해 TFLite 런타임 환경에서 측정을 진행합니다.
따라서, 개발한 Sparse 코드 생성 기술은 TFLite에 포팅하였으며, 포팅이 완료된 TFLite를 기반으로 benchmark app을 빌드하였습니다. 빌드된 app은 ```mlbench``` 디렉토리에 포함되어 있습니다:
* ```benchmark_model_aarch64```: IoT 장치용
* ```benchmark_model_android```: 스마트폰용용

### Baseline 모델 다운로드
TensorFlow [Hosted Models](https://www.tensorflow.org/lite/guide/hosted_models) 페이지에서 [Mobilenet_V2_1.0_224](https://storage.googleapis.com/download.tensorflow.org/models/tflite_11_05_08/mobilenet_v2_1.0_224.tgz) 모델 다운로드 받아 이를 baseline으로 사용합니다.

## [Device] ML Benchmark 수행
모델 및 benchmark 앱을 IoT 장치 또는 스마트폰에 업로드한 후, 아래 명령어를 통해 실행 시간 측정을 진행합니다:
```bash
./benchmark_model --graph=mobilenet_v2_1.0_224.tflite
./benchmark_model --graph=mbv1_0.9_v4u.tflite
```
각 모델의 평균 실행 시간을 기준으로, 압축된 모델의 성능 향상 정도를 비교합니다.


## 4. 머신 러닝 메모리 요구량

### 모델 메모리 요구량 측정
3번 실험에서 생성된 ```tflite``` 모델의 파일 크기를 확인하여, 모델의 파라미터 메모리 요구량을 측정합니다.


## 5. Tiny 머신 러닝 메모리 요구량

### 모델 학습 및 평가
```bash
cd Auto-Compression/tinyml/patch-aware-training
./patch_aware_trainer.sh
```
학습된 모델의 정확도 및 peak memory를 확인합니다.


## 6. 지원 개발 보드 수

### 보드 호환성 검증
ANT API로 구성된 테스트 케이스 ```compatibility/streaming.js```를 수행하여, 통과하는지 확입힙니다. 해당 테스트를 통과할 경우 그 장치는 ANT 프레임워크와 호환됨을 의미합니다.
* Passed Devices:
    - Raspberry Pi 2/3/4/5
    - ODROID XU3/XU4
    - Jetson TX1/TX2/Xavier


## 7. 지원 NPU 수

### 컴파일러 소스 다운로드 및 설정
```bash
git clone https://github.com/SKKU-ESLAB/ANT-Model-DB.git
cd ANT-Model-DB/compiler
mkdir build
cp cmake/config.cmake build/
```

```build/config.cmake``` 파일 안의 설정을 변경하여 NPU를 지원하도록 합니다.
```cmake
# VTA 설정
set(USE_VTA_FSIM ON)

# Ethos-U55 설정
set(USE_ETHOSU ON)

# GEMMINI 설정
set(USE_GEMMINI ON)
```

### 컴파일러 빌드 및 설치
```bash
cd build
cmake ..
make -j$(nproc)
pip install -e .
```

### NPU 지원 테스트
```bash
# VTA 테스트
python3 testcase/test_vta.py

# ETHOS-U55 테스트
python3 testcase/test_ethosu.py

# GEMMINI 테스트트
python3 testcase/test_gemmini.py
```
* Passed NPUs:
    - VTA
    - Ethos-U55
    - GEMMINI


## 8. Attention 기반 모델 처리량

아래 링크의 README를 기반으로 Token Packing이 적용된 ViT 모델 학습
- https://github.com/SKKU-ESLAB/Auto-Compression/tree/master/PackViT

학습된 모델을 ```tflite``` 포맷으로 변화하여 벤치마크 수행