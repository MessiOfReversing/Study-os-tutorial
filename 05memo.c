커널이란것은 일단 디스크에서 읽혀야하는데, 이는 로딩전 가장 마지막 단계다. 
즉 디스크에 올라가기전 가장 마지막 단계인데, 함수 호출과, 스트럭쳐 제어, 문자열 다루기 등등이다.


문자열을 어떻게 다룰것인가?

04에선 A, B, C, X같은 캐릭터는 다뤄봤는데 스트링도 별반 다르지 않다.
그대신 C처럼 문자열은 항상 null byte로 종결해야 옳다.

mystring:
    db 'Hello, World', 0

가운데 quotes 부분은 아스키로 변형된다. 마지막은 0x00으로 읽히는식이다.

cmp ax, 4      ; if ax = 4 와 의미가 같다. cmp가 compare인데, 같냐 아니냐를 판단한다.
je ax_is_four  ; 4가 맞다면 ax_is_four로 jmp를 하며, 거기서 시키는 일을 추가로 진행한다.
jmp else       ; else, 즉 4가 아니라면 또 다른일을 받아서 진행한다.
jmp endif      ; finally, resume the normal flow

ax_is_four:
    .....
    jmp endif

else:
    .....
    jmp endif  ; not actually necessary but printed here for completeness

endif:
