커널이란것은 일단 디스크에서 읽혀야하는데, 이는 로딩전 가장 마지막 단계다. 
즉 디스크에 올라가기전 가장 마지막 단계인데, 함수 호출과, 스트럭쳐 제어, 문자열 다루기 등등이다.


문자열을 어떻게 다룰것인가?

04에선 A, B, C, X같은 캐릭터는 다뤄봤는데 스트링도 별반 다르지 않다.
그대신 C처럼 문자열은 항상 null byte로 종결해야 옳다.

mystring:
    db 'Hello, World', 0

가운데 quotes 부분은 아스키로 변형된다. 마지막은 0x00으로 읽히는식이다.

어샘블리 코드는 늘 이런식일 것이다

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

어샘블리의 문법을 감안하며 코드를 보면 if는 물론이고 =나 !=이나 이런저런 jmp 컨디션들이 직관적으로 받아들여질것이다.

이제 함수를 호출해보자.

위에서 봤듯이 함수를 호출한다는건 정해진 블록으로 점프 컨디션을 작동시켜주는것에 지나지 않는다.
문제는 매개변수인데, 두가지 스텝으로 고려할것이 있는데,
1. 함께 사용하도록 공유할 레지스터와 메모리 주소가 필요하다.
2. 코드를 작성하고 함수를 만들때에는 제너릭을 추가 효과 없이 호출한다.

mov al, 'X'
jmp print
endprint:

...

print:
    mov ah, 0x0e  ; tty code
    int 0x10      ; I assume that 'al' already has the character
    jmp endprint  ; this label is also pre-agreed

이렇게 print 함수를 쓸때는 jmp한뒤 endprint로 복귀시키면 끝나게 된다. 만약 다른 함수도 함수 내에서 또 print를 호출하고 싶어한다면 어떨까? 당연하지만 재활용하면 된다. 이 재활용을 위해서 준비해둘것을 알아보자.

1. 리턴 주소를 저장한다.
2. 최근 레지스터는 저장을 할것이며, 이는 서브함수가 사이드효과 없이 작동하도록 돕는다.

리턴주소를 저장하기 위해서는 cpu가 도와야한다. jmp만 주구장창 쓰는건 심각한 오버헤드를 유발한다. jmp대신 call이나 ret을 이용하는 방식을 택해보도록 하자.
레지스터가 데이터를 저장하기 위해서는 특별한 명령어가 더 있는데, 스택에 접근하는 명령어다. pusha, popa이다. push로 들어가서 pop으로 빼내는 push/pop구조는 익숙할텐데, 다르지 않다. 그대신 a가 붙으니 그냥 넣고 빼는게 아니라 "모든 레지스터를 스택에" 넣고 뺀다는 의미가 된다. 

%include "file.asm"
어샘블리도 다행히 헤더쓰듯이 특정 파일에 선언해둔 여러 함수들을 이 한줄로 갖다쓰는게 가능하다.

