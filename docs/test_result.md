# Gilded Rose Test Result

## 1. 실행 개요

`GildedRose::updateQuality()`에 대한 Google Test 기반 단위 테스트를 CMake/CTest로 실행했다.

- 실행 일시: 2026-05-19
- 실행 위치: `cpp`
- 테스트 프레임워크: Google Test
- 빌드 도구: CMake
- 대상 테스트 파일: `cpp/test/GildedRoseTest.cpp`

## 2. 실행 명령

PowerShell 환경에서 빌드가 성공한 경우에만 CTest를 실행하도록 다음 명령을 사용했다.

```powershell
cmake --build build; if ($LASTEXITCODE -eq 0) { ctest --test-dir build --output-on-failure }
```

## 3. 전체 결과

빌드와 테스트 모두 성공했다.

```text
100% tests passed, 0 tests failed out of 26
Total Test time (real) = 0.43 sec
```

## 4. 테스트 범위별 결과

총 26개 테스트가 실행되었고 모두 통과했다.

- Normal: 5개 통과
- Aged Brie: 5개 통과
- Backstage Pass: 6개 통과
- Sulfuras: 5개 통과
- Conjured: 5개 통과

## 5. 검증된 주요 규칙

이번 테스트 실행에서 다음 규칙을 검증했다.

- Normal 아이템은 판매 기한 전에는 `quality`가 1 감소하고, 판매 기한 이후에는 2 감소한다.
- Aged Brie는 시간이 지날수록 `quality`가 증가하며, 판매 기한 이후에는 2 증가한다.
- Backstage Pass는 콘서트 날짜가 가까워질수록 증가량이 커지고, 콘서트 이후에는 `quality`가 0이 된다.
- Sulfuras는 `sellIn`과 `quality`가 변하지 않는다.
- Conjured 아이템은 Normal 아이템보다 두 배 빠르게 품질이 감소한다.
- `quality`는 일반 아이템 기준으로 0 미만 또는 50 초과가 되지 않는다.
- `sellIn == 0`, `sellIn == -1`, `quality == 0`, `quality == 50` 경계값이 기대대로 동작한다.

## 6. 결론

현재 테스트 세트는 Green 상태다.

`GildedRose::updateQuality()`의 주요 아이템 타입별 품질 및 판매 기한 규칙은 CTest 기준으로 모두 통과했다.
