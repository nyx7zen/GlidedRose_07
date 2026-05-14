# Gilded Rose

Gilded Rose 는 게임 "World of Warcraft"에 나오는 여관 이름입니다.  
아이템은 퀄리티 속성을 가지고 있으며, 예외 사항이 있는 아이템들이 있습니다.  
- 아이템은 [아이템 이름, 퀄리티, 유통기한]의 속성을 가집니다.  
- 아이템의 퀄리티는 0 이상이고 아이템의 퀄리티는 하루가 지날 때마다 1씩 줄어듭니다.  
- 유통 기한이 지난 아이템의 퀄리티는 2배의 속도로 떨어집니다.  
- 퀄리티는 최대값이 50입니다.  

### Aged Brie, Backstage Pass, Sulfuras 의 예외적 규칙  

- Aged Brie는 하루가 지날 때마다 퀄리티가 1씩 증가합니다.  
- 유통기한이 지나면 퀄리티가 2씩 증가합니다.  
- Backstage Pass 는 유통기한(콘서트일)이 다가올수록 퀄리티가 증가합니다.  
  유통기한이 11일 이상일 때는 1, 10일 이하일 때는 2, 5일 이하일 때는 3씩 증가하지만  
  콘서트 날이 지나면 퀄리티는 0이 됩니다.  
- Sulfuras의 퀄리티는 변화가 없습니다.  
  
---------------------------------------------------------  
## 실습 목적 :  테스트 코드 작성을 통한 legacy code 이해

### 프로젝트 구조 (C++)

```
cpp/
├── CMakeLists.txt          # CMake 빌드 설정 (Google Test 자동 다운로드)
├── include/
│   ├── Item.h              # Item 클래스 (수정 불가)
│   └── GildedRose.h        # GildedRose 클래스 헤더
├── src/
│   └── GildedRose.cpp      # updateQuality() 구현
└── test/
    └── GildedRoseTest.cpp  # Google Test 기반 테스트
```

### 빌드 및 테스트 실행

```bash
cd cpp
cmake -B build
cmake --build build
ctest --test-dir build
```

### GildedRoseTest.cpp : failed test 수정
- updateQuality() 의 test case를 추가 작성해 보세요.
- unit test 내용 : 문서상에 나타난 동작들 확인

### unit test VS golden-master test 비교

### To-Do List (dev branch)

- [x] Foo — 기본 테스트 (name 불변 확인)
- [x] noname_sellin_0_quality_0 — 일반 아이템: 품질이 이미 0이라 감소 안 됨
- [x] noname_sellin_0_quality_5 — 일반 아이템: 유통기한 지나면 품질 -2
- [x] sulfuras_sellin_0_quality_5 — 전설 아이템: 아무것도 변화 없음
- [x] sulfuras_sellin_m1_quality_5 — 전설 아이템: 기한 마감 후에도 변화 없음
- [x] agedBrie_sellin_0_quality_0 — Aged Brie: 기한 지나면 품질 +2
- [x] agedBrie_sellin_0_quality_50 — Aged Brie: 품질 상한 50 초과 불가
- [x] backstage_pass_sellin_15_quality_0 — Backstage: 10일 초과 시 품질 +1
- [x] backstage_pass_sellin_0_quality_0 — Backstage: 공연 지난 후 품질 0
- [x] backstage_pass_sellin_0_quality_51 — Backstage: 품질 초과 후 공연 종료 시 0
- [x] should_be_nothing_when_no_item — 빈 배열: 아이템 없을 경우 정상 처리
- [x] ApprovalTest ThirtyDays — Golden Master 30일 스냅샷 승인
---

### To-Do List (refactoring branch)

#### Step 1 — 테스트 먼저 (회귀 테스트 보강)
- [x] NormalItemDegrades — 일반 아이템: 매일 quality -1, sellIn -1
- [x] AgedBrieIncreasesQuality — Aged Brie: 오래될수록 quality 증가
- [x] BackstagePass_SellIn11_QualityPlus1 — Backstage: sellIn 11일 경계값 +1
- [x] BackstagePass_SellIn10_QualityPlus2 — Backstage: sellIn 10일 경계값 +2
- [x] BackstagePass_SellIn6_QualityPlus2 — Backstage: sellIn 6일 +2
- [x] BackstagePass_SellIn5_QualityPlus3 — Backstage: sellIn 5일 경계값 +3

#### Step 2 — 코드 정리 (상수 추출 + 변수 추출)
- [x] AGED_BRIE, BACKSTAGE_PASS, SULFURAS 상수 추출
- [x] MAX_QUALITY, MIN_QUALITY 상수 추출
- [x] items[i] → Item& item 변수 추출
- [x] 불필요한 중첩 제거

#### Step 3 — 조건 분리 (Condition Simplification)
- [x] if(!~) → 긍정 조건으로 전환 (Invert if)
- [x] else + if → else if 병합
- [x] sellIn 전/후 중복 조건 하나로 합치기 (sellIn 0 → 1 경계 조정)

#### Step 4 — 메서드 추출 (Extract Method)
- [x] updateAgedBrie() 추출
- [x] updateBackstagePass() 추출
- [x] updateSulfuras() 추출
- [x] updateNormalItem() 추출
- [x] updateSellIn() 추출

#### Step 5 — 클래스 분리 (Move Method + New Class)
- [x] AgedBrieItem 클래스 생성
- [x] BackstagePassItem 클래스 생성
- [x] SulfurasItem 클래스 생성
- [x] NormalItem 클래스 생성

#### Step 6 — 추상화 (Abstract Base Class + Factory)
- [x] GildedRoseItem 추상 클래스 생성
- [x] 각 클래스 GildedRoseItem 상속
- [x] createItem() Factory 함수 생성
- [x] updateQuality() 다형성 적용

#### Step 7 — 새 기능 추가 및 ApprovalTest 갱신 (Food & Beverage)
- [x] FoodBeverageItem 클래스 생성
- [x] Factory 에 F&B 타입 등록
- [x] F&B 테스트 추가 (DegradesTwiceAsNormal)
- [x] F&B 테스트 추가 (DegradesFourTimesAfterSellIn)
- [x] F&B 테스트 추가 (QualityNeverBelowZero)
- [x] ApprovalTest.ThirtyDays approved.txt 갱신

#### Step 8 — 변수 선언 분리 (Split Variable Declaration)
- [x] unique_ptr 선언과 초기화 분리
- [x] if/else if 체인으로 통일

#### Step 9 — 중복 제거 (Remove Duplication)
- [ ] createItem() 으로 Factory 로직 추출
- [ ] updateQuality() 내 중복 제거

#### Step 10 — Extract Method 정리
- [ ] updateQuality() 타입별 로직 각 클래스에 위임 확인
- [ ] updateSellIn() 분리 유지 확인

#### Step 11 — Factory 조기 반환 패턴 (Early Return)
- [ ] else if 체인 → if 조기 반환 패턴으로 변경

#### Step 12 — Inline Variable (임시 변수 제거)
- [ ] gi 임시 변수 제거
- [ ] createItem(item)->updateQuality() 한 줄로 정리