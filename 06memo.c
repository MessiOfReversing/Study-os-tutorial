메모리 주소 매기는법과, 리얼모드 분할을 16비트로 배워보자.

cs, ds, ss, es라는 레지스터를 이용한다. 각각 code, data, stack, extra(user-defined같은) 을 의미한다.
레지스터를 다루다보니 CPU가 가감없이 바로 읽는단걸 감안해야하는데, 최초 값 선언에선 ds를 쓴다는걸 유의하자. ds를 쓰면 이 값에따라서 우리가 메모리에 접근할때마다 ds값에 맞게 오프셋이 설정된다.

real address 계산을 위해서는 두 주소를 join하는것만이 아니라 overlap하는것이라 이해하는게 옳다.
segment << 4 + address 같은 방식을 쓰는것인데, 만약 ds가 0x4d에 있다면, [0x20]를 입력할시에 0x4d0 + 0x20 = 0x4f0 로 계산하는것이다.

mov ah, 0x0e ; tty

mov al, [the_secret]
int 0x10 ; we already saw this doesn't work, right?

mov bx, 0x7c0 ; remember, the segment is automatically <<4 for you
mov ds, bx
; WARNING: from now on all memory references will be offset by 'ds' implicitly
mov al, [the_secret]
int 0x10

mov al, [es:the_secret]
int 0x10 ; doesn't look right... isn't 'es' currently 0x000?

mov bx, 0x7c0
mov es, bx
mov al, [es:the_secret]
int 0x10


jmp $

the_secret:
    db "X"

times 510 - ($-$$) db 0
dw 0xaa55
