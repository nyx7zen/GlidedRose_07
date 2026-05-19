# Gilded Rose Code Quality Report

## updateQuality() SOLID / Code Smell 분석

| 문제점 | 위반 원칙/스멜 | 영향 | 개선 방향 | 우선순위 |
|---|---|---|---|---:|
| `updateQuality()`가 모든 아이템 타입의 판별, 품질 계산, 판매 기한 감소, 경계값 보정을 한 메서드에서 처리한다. | SRP 위반, Long Method | 아이템 규칙 하나를 바꿀 때 전체 메서드를 다시 이해해야 하며, 작은 변경도 다른 아이템 동작을 깨뜨릴 위험이 크다. | 아이템 타입별 업데이트 정책을 분리한다. `Normal`, `Aged Brie`, `Backstage Pass`, `Sulfuras`, `Conjured` 각각의 규칙을 별도 함수 또는 전략 객체로 이동한다. | 1 |
| `"Aged Brie"`, `"Backstage passes to a TAFKAL80ETC concert"`, `"Sulfuras, Hand of Ragnaros"` 문자열 비교가 메서드 곳곳에 흩어져 있다. | OCP 위반, Primitive Obsession, Duplicated Code | 새 아이템 타입을 추가할 때 기존 조건문을 계속 수정해야 한다. 이름 오타나 비교 정책 변경도 여러 위치를 동시에 고쳐야 한다. | 아이템 이름 상수 또는 타입 판별 함수를 도입한다. 이후 `ItemUpdater` 전략 선택 로직이나 규칙 테이블에서 한 번만 분류한다. | 1 |
| 품질 상한/하한과 임계일이 `0`, `50`, `11`, `6` 같은 숫자로 직접 표현되어 있다. | Magic Number | `11`은 "10일 이하", `6`은 "5일 이하"를 구현한 값이라 요구사항과 코드 사이의 의미 변환을 독자가 추론해야 한다. 정책 변경 시 누락 가능성이 높다. | `MinQuality = 0`, `MaxQuality = 50`, `BackstageDoubleIncreaseThreshold = 10`, `BackstageTripleIncreaseThreshold = 5`, `SulfurasQuality = 80` 같은 명명된 상수를 둔다. 비교식은 `sellIn <= threshold`처럼 요구사항 언어와 맞춘다. | 2 |
| 품질 증가/감소 전에 매번 `quality > 0`, `quality < 50`을 중첩 조건문으로 반복한다. | Duplicated Code, Shotgun Surgery | 경계값 보정 로직이 분산되어 있고, `Conjured`처럼 감소량이 달라지는 규칙을 넣으면 반복 코드가 더 늘어난다. | `increaseQuality(item, amount)`, `decreaseQuality(item, amount)`, `clampQuality()` 같은 작은 헬퍼로 경계값 보정을 한 곳에 모은다. | 2 |
| 일반 아이템 처리와 만료 후 일반 아이템 처리가 별도 조건문 블록에 중복 구현되어 있다. | Duplicated Code, Conditional Complexity | "일반 아이템은 만료 후 두 배 감소"라는 하나의 정책이 두 군데에 나뉘어 표현된다. 감소율 변경이나 `Conjured` 추가 시 분기가 급격히 복잡해진다. | 각 전략에서 먼저 `qualityDelta`를 계산하고 공통 헬퍼로 적용한다. 예: 일반 아이템은 `sellIn <= 0 ? -2 : -1`, Conjured는 `sellIn <= 0 ? -4 : -2`. | 2 |
| `Sulfuras` 예외 처리가 품질 감소 블록과 `sellIn` 감소 블록에 반복적으로 등장한다. | OCP 위반, Duplicated Code, Special Case Smell | 불변 아이템이라는 핵심 규칙이 코드 여러 지점에 퍼져 있어 누락 위험이 있다. | `SulfurasUpdater`는 아무 것도 하지 않는 전략으로 분리한다. 또는 루프 초반에 `if (isSulfuras(item)) continue;`로 예외 흐름을 명확히 한다. | 3 |
| `if`/`else` 중첩이 깊고 부정 조건(`!=`)이 많다. | Long Method, Nested Conditionals, Readability Smell | 읽는 사람이 정상 흐름과 예외 흐름을 동시에 추적해야 한다. 조건 추가 시 실수로 잘못된 분기에 끼워 넣기 쉽다. | 가드 클로즈, 명명된 predicate(`isAgedBrie`, `isBackstagePass`, `isSulfuras`)와 타입별 함수로 조건 깊이를 낮춘다. | 3 |
| `items[i]` 인덱스 접근이 메서드 전체에 반복된다. | Readability Smell, Low-level Iteration | 같은 아이템을 다루고 있다는 의도가 흐려지고 코드가 장황해진다. | C++17 범위 기반 for문 `for (Item& item : items)`를 사용한다. 각 전략/헬퍼는 `Item&`를 받아 동작하게 한다. | 4 |
| 신규 요구사항인 `Conjured`를 수용할 확장 지점이 없다. | OCP 위반, Feature Envy | 새 타입을 추가하려면 기존 메서드의 여러 조건식을 수정해야 하므로 회귀 위험이 높다. | 우선 전략 패턴 또는 함수 테이블을 도입한다. `std::unordered_map<std::string_view, Updater>`나 `std::vector<Rule>` 기반 선택 로직으로 타입 추가 지점을 제한한다. | 4 |
| 아이템 타입이 문자열로만 표현되어 컴파일 타임 안전성이 낮다. | Primitive Obsession, Type Safety Smell | 오타, 부분 매칭, 이름 정책 변경이 런타임 버그로 나타난다. 단, `Item` 클래스 변경 금지 제약 때문에 완전한 타입 모델링은 어렵다. | `Item`을 바꾸지 않는 선에서 `enum class ItemKind` 분류 함수를 둘 수 있다. 더 큰 리팩토링에서는 `std::variant<Normal, AgedBrie, BackstagePass, Sulfuras, Conjured>` 기반 도메인 모델을 고려한다. | 5 |

## 개선 방향 요약

1. 가장 먼저 `updateQuality()` 안의 반복되는 경계값 보정과 아이템 이름 비교를 상수/헬퍼/predicate로 분리한다.
2. 다음 단계에서 아이템별 품질 변화 규칙을 `updateNormal`, `updateAgedBrie`, `updateBackstagePass`, `updateSulfuras`, `updateConjured` 같은 작은 함수로 나눈다.
3. 새 아이템 타입이 계속 추가될 가능성이 있다면 전략 패턴을 적용한다. C++17에서는 공통 인터페이스 기반 클래스, 함수 객체, 또는 `std::function<void(Item&)>` 테이블을 사용할 수 있다.
4. 아이템 종류가 고정적이고 규칙이 단순하다면 테이블 기반 접근도 적합하다. 이름 또는 `ItemKind`를 key로 updater 함수를 선택하면 `updateQuality()`는 순회와 위임만 담당한다.
5. `std::variant`는 컴파일 타임 타입 안전성을 높일 수 있지만, 현재 요구사항의 "Item 클래스/Items 속성 변경 금지" 제약과 충돌할 가능성이 있다. 따라서 단기 리팩토링보다는 장기적인 도메인 모델 개선안으로 두는 것이 적절하다.

## 권장 리팩토링 순서

| 순서 | 작업 | 이유 |
|---:|---|---|
| 1 | 기존 동작을 고정하는 테스트를 먼저 확보한다. | 레거시 조건문 리팩토링은 회귀 위험이 크며, 특히 Backstage Pass와 만료 후 규칙이 깨지기 쉽다. |
| 2 | Magic Number와 아이템 이름 문자열을 상수화한다. | 동작 변경 없이 의미를 드러낼 수 있는 가장 안전한 개선이다. |
| 3 | 품질 증감/상하한 보정 헬퍼를 만든다. | 중복을 줄이고 `Conjured`의 다른 감소량도 같은 경로로 처리할 수 있다. |
| 4 | 아이템별 업데이트 함수를 분리한다. | SRP를 회복하고 `updateQuality()`의 조건문 복잡도를 크게 낮춘다. |
| 5 | 전략 패턴 또는 테이블 기반 디스패치를 도입한다. | OCP를 만족하는 확장 지점을 만들 수 있지만, 구조 변경 폭이 커서 테스트와 작은 함수 분리가 선행되어야 한다. |
