# Gilded Rose Test Plan

## 1. 목적과 범위

이 문서는 C++17, Google Test, CMake 기반의 Gilded Rose 코드에 대한 테스트 전략을 정의한다. 핵심 목표는 기존 레거시 동작을 안전하게 고정하고, 신규 요구사항인 `Conjured` 아이템을 포함한 비즈니스 규칙을 회귀 테스트로 보호하는 것이다.

테스트 대상은 `GildedRose::updateQuality()`이며, `Item` 클래스와 `GildedRose::items` 속성은 요구사항상 외부 계약으로 간주하여 구조 변경 없이 검증한다.

## 2. 테스트 접근 방식

| 테스트 유형 | 목적 | 도구 | 우선순위 |
|---|---|---|---:|
| `TEST_F` 기반 단위 테스트 | 아이템 타입별 `sellIn`, `quality` 변화 규칙 검증 | Google Test | P0 |
| 경계값 테스트 | `quality` 하한/상한, 만료일 전후 조건 검증 | Google Test | P0 |
| 예외/특이 케이스 테스트 | `Sulfuras`, `Backstage Pass` 만료, 미지원/알 수 없는 이름 처리 검증 | Google Test | P0 |
| 조합 테스트 | 여러 아이템을 한 번에 업데이트할 때 상호 영향이 없는지 검증 | Google Test | P1 |
| 승인/스냅샷 테스트 | 리팩토링 전후 전체 출력 회귀 검증 | ApprovalTests 또는 텍스트 fixture | P1 |
| 커버리지 측정 | 분기 누락과 리스크 높은 조건문 식별 | gcov/lcov | P1 |

## 3. `TEST_F` 기반 단위 테스트 구조

현재 `cpp/test/GildedRoseTest.cpp`의 placeholder 테스트는 실제 요구사항을 검증하지 못하므로, 공통 fixture를 사용해 테스트 중복을 줄인다.

권장 fixture 책임:

- 단일 아이템 업데이트 헬퍼 제공
- 여러 아이템 업데이트 헬퍼 제공
- 결과 검증 시 `name`, `sellIn`, `quality`를 함께 확인
- 아이템 이름 상수를 테스트 코드에 모아 오타 리스크 감소

예상 구조:

```cpp
class GildedRoseTest : public ::testing::Test {
protected:
    Item updateOne(const std::string& name, int sellIn, int quality);
    std::vector<Item> updateMany(std::vector<Item> items);
};
```

테스트 그룹은 다음 순서로 구성한다.

| 우선순위 | 테스트 그룹 | 주요 검증 |
|---:|---|---|
| P0 | Normal item | 기본 감소, 만료 후 2배 감소, `quality` 0 하한 |
| P0 | Aged Brie | 기본 증가, 만료 후 2배 증가, `quality` 50 상한 |
| P0 | Backstage Pass | 11일 이상, 10일 이하, 5일 이하, 콘서트 이후 0 |
| P0 | Sulfuras | `sellIn`, `quality` 모두 불변, `quality` 80 예외 |
| P0 | Conjured | 만료 전 -2, 만료 후 -4, `quality` 0 하한 |
| P1 | Unknown item name | 일반 아이템과 동일하게 처리되는지 검증 |
| P1 | Multiple items | 한 번의 업데이트에서 각 아이템 규칙이 독립적으로 적용되는지 검증 |

## 4. 아이템 타입별 단위 테스트 범위

### 4.1 Normal Item

| 케이스 | Given | Expected |
|---|---|---|
| 일반 감소 | `sellIn=10`, `quality=20` | `sellIn=9`, `quality=19` |
| 만료일 도달 | `sellIn=0`, `quality=20` | `sellIn=-1`, `quality=18` |
| 이미 만료 | `sellIn=-1`, `quality=20` | `sellIn=-2`, `quality=18` |
| 하한 유지 | `sellIn=10`, `quality=0` | `sellIn=9`, `quality=0` |
| 하한 보정 | `sellIn=0`, `quality=1` | `sellIn=-1`, `quality=0` |

### 4.2 Aged Brie

| 케이스 | Given | Expected |
|---|---|---|
| 일반 증가 | `sellIn=10`, `quality=20` | `sellIn=9`, `quality=21` |
| 만료일 도달 | `sellIn=0`, `quality=20` | `sellIn=-1`, `quality=22` |
| 이미 만료 | `sellIn=-1`, `quality=20` | `sellIn=-2`, `quality=22` |
| 상한 직전 | `sellIn=10`, `quality=49` | `sellIn=9`, `quality=50` |
| 상한 유지 | `sellIn=10`, `quality=50` | `sellIn=9`, `quality=50` |
| 만료 후 상한 보정 | `sellIn=0`, `quality=49` | `sellIn=-1`, `quality=50` |

### 4.3 Backstage Pass

| 케이스 | Given | Expected |
|---|---|---|
| 11일 이상 | `sellIn=11`, `quality=20` | `sellIn=10`, `quality=21` |
| 10일 이하 | `sellIn=10`, `quality=20` | `sellIn=9`, `quality=22` |
| 5일 이하 | `sellIn=5`, `quality=20` | `sellIn=4`, `quality=23` |
| 콘서트 당일 처리 | `sellIn=0`, `quality=20` | `sellIn=-1`, `quality=0` |
| 이미 콘서트 지남 | `sellIn=-1`, `quality=20` | `sellIn=-2`, `quality=0` |
| 상한 직전 10일 이하 | `sellIn=10`, `quality=49` | `sellIn=9`, `quality=50` |
| 상한 직전 5일 이하 | `sellIn=5`, `quality=49` | `sellIn=4`, `quality=50` |
| 상한 유지 | `sellIn=5`, `quality=50` | `sellIn=4`, `quality=50` |

### 4.4 Sulfuras

| 케이스 | Given | Expected |
|---|---|---|
| 기본 불변 | `sellIn=10`, `quality=80` | `sellIn=10`, `quality=80` |
| 만료일 불변 | `sellIn=0`, `quality=80` | `sellIn=0`, `quality=80` |
| 만료 후 불변 | `sellIn=-1`, `quality=80` | `sellIn=-1`, `quality=80` |
| 일반 상한 예외 | `quality=80` | 50으로 보정하지 않음 |

### 4.5 Conjured

`Conjured` 이름 판별 정책은 테스트에서 명확히 고정해야 한다. 요구사항 문구상 `"Conjured Mana Cake"` 같은 이름을 지원할 가능성이 높으므로, 제품 정책 결정 전까지는 다음 두 케이스를 별도 테스트로 분리한다.

| 케이스 | Given | Expected |
|---|---|---|
| 정확한 이름 매칭 | `name="Conjured"`, `sellIn=10`, `quality=20` | `sellIn=9`, `quality=18` |
| 포함 이름 매칭 | `name="Conjured Mana Cake"`, `sellIn=10`, `quality=20` | 정책 결정에 따라 `quality=18` 또는 일반 아이템 `quality=19` |
| 만료일 도달 | `sellIn=0`, `quality=20` | `sellIn=-1`, `quality=16` |
| 이미 만료 | `sellIn=-1`, `quality=20` | `sellIn=-2`, `quality=16` |
| 하한 직전 | `sellIn=10`, `quality=1` | `sellIn=9`, `quality=0` |
| 만료 후 하한 보정 | `sellIn=0`, `quality=3` | `sellIn=-1`, `quality=0` |

## 5. 경계값 케이스 목록

아래 경계값은 모든 관련 아이템 타입에 대해 우선 적용한다. 특히 `quality` 증감량이 다른 `Normal`, `Aged Brie`, `Backstage Pass`, `Conjured`는 각각 별도 테스트가 필요하다.

### 5.1 `quality` 경계값

| `quality` | 검증 목적 | 대상 |
|---:|---|---|
| 0 | 감소형 아이템이 음수로 내려가지 않음 | Normal, Conjured |
| 1 | 감소량이 현재 품질보다 클 때 0으로 보정 | Normal, Conjured |
| 49 | 증가형 아이템이 50을 초과하지 않음 | Aged Brie, Backstage Pass |
| 50 | 증가형 아이템 상한 유지 | Aged Brie, Backstage Pass |
| 80 | `Sulfuras` 전용 예외 값 유지 | Sulfuras |

### 5.2 `sellIn` 경계값

| `sellIn` | 검증 목적 | 대상 |
|---:|---|---|
| 0 | 업데이트 후 만료 상태가 되며 만료 후 규칙 적용 여부 검증 | Normal, Aged Brie, Backstage Pass, Conjured |
| -1 | 이미 만료된 아이템의 규칙 검증 | Normal, Aged Brie, Backstage Pass, Conjured |
| 11 | Backstage Pass +1 구간 마지막 값 | Backstage Pass |
| 10 | Backstage Pass +2 구간 시작 값 | Backstage Pass |
| 6 | Backstage Pass +2 구간 마지막 값 | Backstage Pass |
| 5 | Backstage Pass +3 구간 시작 값 | Backstage Pass |

## 6. 예외/특이 케이스 목록

| 케이스 | 리스크 | 기대 검증 |
|---|---|---|
| `Sulfuras`는 `quality=80` | 일반 상한 50 규칙을 잘못 적용할 수 있음 | `sellIn`, `quality` 모두 불변 |
| `Backstage Pass`는 콘서트 후 0 | 증가 규칙과 만료 규칙의 충돌 가능 | `sellIn <= 0` 업데이트 후 `quality=0` |
| 알 수 없는 이름 | 문자열 분기 누락 또는 신규 아이템 오분류 | 일반 아이템 규칙으로 처리 |
| `Conjured` 이름 정책 | 정확 매칭/부분 매칭 정책 불명확 | 테스트 이름에 정책을 드러내고 제품 결정 후 고정 |
| 여러 아이템 동시 업데이트 | 참조 기반 `items` 변경이 다른 원소에 영향 줄 수 있음 | 각 아이템이 독립적으로 업데이트 |
| 빈 아이템 목록 | 루프 처리 안정성 | 예외 없이 종료, 목록 크기 유지 |
| 비정상 초기 `quality > 50` | 요구사항은 상한을 말하지만 입력 정합성은 불명확 | 정책 결정 필요. 우선 정상 입력 범위 중심으로 테스트 |
| `Sulfuras`의 비정상 quality | `quality=80`이 계약인지 보정 대상인지 불명확 | 요구사항상 80 고정 입력만 P0로 검증 |

## 7. 커버리지 목표

| 항목 | 목표 |
|---|---:|
| Line coverage | 90% 이상 |
| Branch coverage | 85% 이상 |
| Function coverage | 100% |
| 핵심 비즈니스 규칙 테스트 통과율 | 100% |

현재 `updateQuality()`는 중첩 조건문이 많아 line coverage만으로는 충분하지 않다. `Backstage Pass`의 `sellIn < 11`, `sellIn < 6`, `quality < 50`, 만료 후 `quality=0` 분기를 모두 통과하도록 branch coverage를 함께 확인한다.

## 8. gcov/lcov 측정 전략

GCC 또는 MinGW 환경에서 coverage 빌드를 별도 디렉터리로 분리한다.

```bash
cmake -S cpp -B cpp/build-coverage -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="--coverage -O0 -g"
cmake --build cpp/build-coverage
ctest --test-dir cpp/build-coverage --output-on-failure
lcov --capture --directory cpp/build-coverage --output-file cpp/build-coverage/coverage.info
lcov --remove cpp/build-coverage/coverage.info "*/_deps/*" "*/test/*" --output-file cpp/build-coverage/coverage.filtered.info
genhtml cpp/build-coverage/coverage.filtered.info --output-directory cpp/build-coverage/coverage-html
```

Windows에서 `lcov` 설치가 어렵다면 우선 `gcov` 또는 `gcovr`로 대체 측정한다.

```bash
gcovr -r cpp --object-directory cpp/build-coverage --html --html-details -o cpp/build-coverage/coverage.html
gcovr -r cpp --object-directory cpp/build-coverage --branches --txt
```

## 9. 커버리지 개선 전략

1. P0 아이템 타입별 테스트를 먼저 추가해 핵심 규칙의 line/function coverage를 확보한다.
2. `quality` 0, 1, 49, 50과 `sellIn` 0, -1 경계값 테스트를 추가해 하한/상한 보정 분기를 통과시킨다.
3. Backstage Pass의 11/10/6/5 경계 테스트를 추가해 임계값 분기를 빠짐없이 검증한다.
4. `Sulfuras` 불변 테스트로 예외 분기를 고정한다.
5. `Conjured` 정책 결정 후 정확 매칭 또는 부분 매칭 테스트를 P0로 승격한다.
6. 커버리지 리포트에서 누락된 분기를 확인하고, 요구사항 의미가 있는 테스트만 추가한다.
7. 리팩토링 전에는 승인 테스트로 현재 동작을 고정하고, 리팩토링 후에는 단위 테스트와 승인 테스트를 모두 실행한다.

## 10. 실행 기준

PR 또는 리팩토링 완료 전 최소 기준은 다음과 같다.

- 모든 P0 단위 테스트 통과
- `ctest --test-dir cpp/build --output-on-failure` 통과
- line coverage 90% 이상 또는 미달 사유 문서화
- branch coverage 85% 이상 또는 누락 분기별 보완 계획 작성
- `Conjured` 요구사항 구현 시 관련 테스트가 실패에서 성공으로 전환되는지 확인
