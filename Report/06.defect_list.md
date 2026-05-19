# Gilded Rose Defect List

## Summary

- 작성일: 2026-05-19
- 대상: C++17 / Google Test / CMake 기반 `GildedRose::updateQuality()`
- 현재 테스트 상태: `26/26` 통과, 열린 테스트 실패 없음
- 검증 명령:

```powershell
cmake --build build; if ($LASTEXITCODE -eq 0) { ctest --test-dir build --output-on-failure }
```

## Defects

| ID | Severity | ItemType | Steps | Expected | Actual | Root Cause | Fix Summary |
|---|---|---|---|---|---|---|---|
| DEF-001 | High | Functional / Conjured | `Conjured Mana Cake`을 `sellIn=10, quality=20` 및 `sellIn<=0` 조건으로 생성한 뒤 `updateQuality()`를 실행한다. | Conjured 아이템은 Normal보다 2배 빠르게 감소한다. 판매 기한 전에는 `quality -2`, 만료 후에는 `quality -4`가 적용되고 `sellIn`은 1 감소한다. | 기존 구현에는 Conjured 전용 분기가 없어 Normal 아이템처럼 처리될 수 있었다. 요구사항 기반 Conjured 테스트 추가 시 실패 가능한 상태였다. | 신규 요구사항인 Conjured 타입이 레거시 `updateQuality()` 조건 분기에 반영되지 않았다. | `isConjured()` 판별 함수를 추가하고 Conjured 감소량을 `baseDecrease=2`로 계산하도록 구현했다. 관련 Google Test 5개를 추가해 현재 통과 상태로 확인했다. |
| DEF-002 | Medium | Test Execution / Environment | PowerShell에서 `cmake --build build && ctest --test-dir build --output-on-failure` 형식으로 빌드 및 테스트를 실행한다. | 빌드 성공 시 CTest가 이어서 실행되어야 한다. | 해당 PowerShell 환경에서 `&&` 명령 연결이 파싱 단계에서 실패해 테스트 실행이 차단되었다. | 실행 환경의 PowerShell 문법 호환성에 맞지 않는 명령 연결 방식을 사용했다. | `cmake --build build; if ($LASTEXITCODE -eq 0) { ctest --test-dir build --output-on-failure }` 형식으로 변경해 빌드 성공 시에만 테스트를 실행하도록 했다. |
| DEF-003 | Medium | Code Quality / Maintainability | 신규 아이템 타입 또는 규칙 변경을 위해 `GildedRose::updateQuality()`를 수정한다. | 아이템별 규칙은 독립적으로 변경 가능해야 하며, 한 타입 변경이 다른 타입 동작에 미치는 회귀 위험이 낮아야 한다. | `updateQuality()`가 여전히 아이템 판별, 품질 계산, `sellIn` 감소, 경계값 보정을 한 메서드에서 처리한다. | 레거시 조건문 중심 구조로 인해 SRP/OCP가 약하고, 타입 추가 시 기존 메서드 수정이 필요하다. | 상수, `isConjured()`, `increaseQuality()`, `decreaseQuality()` 헬퍼와 가드 흐름으로 일부 완화했다. 장기적으로는 아이템별 updater 함수 또는 전략 패턴 분리가 필요하다. |
| DEF-004 | Low | Requirements / Classification | Conjured 이름 판별 정책을 확인하고 `"Conjured"`, `"Conjured Mana Cake"`, `"Not Conjured"` 같은 이름을 검증한다. | 어떤 문자열을 Conjured 아이템으로 볼지 명확해야 한다. | 요구사항은 `"Conjured" items`라고만 표현되어 정확 매칭인지 접두어/포함 매칭인지 모호하다. | 아이템 타입이 문자열로만 표현되고, 신규 타입의 이름 매칭 정책이 별도 명세로 고정되지 않았다. | 현재 구현은 `"Conjured"` 접두어 기반으로 처리하며, 테스트는 `"Conjured Mana Cake"` 기준으로 고정했다. 추후 제품 정책상 정확 매칭이 필요하면 요구사항과 테스트를 함께 갱신해야 한다. |

