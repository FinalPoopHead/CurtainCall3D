# Cube Runner

---

# 게임 설명

> 2024-07-10 업데이트
> 

## 조작

### 메인 메뉴

좌우 방향키로 메뉴 이동

Enter로 메뉴 선택

### 인게임

w,a,s,d 로 캐릭터 움직임

j : 지뢰 설치 / 작동

k : advantage Mine 작동

l : 빨리 감기 (8배속)

BackSpace : 메인 메뉴로 이동

1~9 숫자키 : 스테이지 이동

## 큐브

Normal Cube : 일반 큐브 (회색)

- 끝에 도달하기 전에 제거해야 한다.
- 제거하지 못하고 끝에 도달한다면 우측 하단 카운트 증가.
- 제거한다면 100점.

Advantage Cube : 어드밴티지 큐브 (초록색)

- 끝에 도달하기 전에 제거해야 한다.
- 제거하지 못하고 끝에 도달한다면 우측 하단 카운트 증가.
- 제거한다면 100점.
- 제거하면 해당 위치에 Advantage Mine이 설치된다.

Dark Cube : 다크 큐브 (검은색)

- 제거하면 안된다.
- 제거하면 타일 한 줄 삭제.
- 제거한다면 점수 -1000.

## 지뢰

Mine : 지뢰 (파란색)

- 플레이어가 임의로 설치할 수 있는 지뢰.
- 동시에 최대 1개만 설치 가능하다.
- 이미 설치된 지뢰가 있는 상태에서 설치버튼을 다시 누르면 설치된 지뢰를 동작시킨다.
- 동작시킨 지뢰 위로 큐브가 도착하면 해당 큐브를 제거한다.

Advantage Mine : 어드밴티지 지뢰 (초록색)

- Advantage Cube를 제거한 위치에 설치되는 지뢰.
- 필드 위에 여러개가 존재할 수 있다.
- 플레이어가 Advantage Mine 동작 버튼을 누르면 맵에 존재하는 모든 Advantage Mine을 동작시킨다.
- 동작하면 해당 위치를 중심으로 3x3 영역에 폭파 지역을 설정한다.
- 폭파 지역에 큐브가 도착하면 해당 큐브를 제거한다.

## 타일 삭제 조건

다크 큐브를 플레이어가 지뢰를 통해 제거하거나,

우측하단의 카운트가 가득 찬 상태에서 큐브를 떨어뜨릴 경우.

# 점수 계산 방법

기본 점수 = 제거한 큐브 개수 * 100

~ 3 개 : 기본 점수

4 ~ 6 개 : 기본 점수 * 2 점

7 ~ 9 개 : 기본 점수 * 4 점

10 개 이상 : 기본 점수 * 8 점