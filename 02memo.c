; Infinite loop (e9 fd ff)
loop:
    jmp loop 

; Fill with 510 zeros minus the size of the previous code
times 510-($-$$) db 0
; Magic number
dw 0xaa55 

이거에서 계속 무한루프를 걸어두었으니 그 루프하는 동안 뭘 시킬것이냐가 작성되어야 할것임.
"Hello"를 레지스터에 집어넣어서 지켜보도록 해보자.

완성된 코드는 아니지만..
mov ah, 0x0e    ; 고차원 자릿수(AH)에 "글자를 화면에 출력해라"라는 명령어 번호(0x0e)를 세팅
mov al, 'H'     ; 저차원 자릿수(AL)에 출력하고 싶은 글자인 'H'를 세팅
int 0x10        ; BIOS에게 AX에 세팅된 대로 모니터에 글자를 띄울것을 명령 (인터럽트 발생)

이런식이다.

그래서 뭘할것이냐 를 정리하자면.. 
Hello 라는 문자열의 각각의 캐릭터를 갖다가 al에 넣고, 0x0e를 ah로, 그리고 0x10이라는 인터럽트를 발생시켜서 비디오 서비스를 시킨다.

0x0e가 ah에 올라가 있으면 "우리가 작동시키고자하는 그 진짜 함수인 비디오 인터럽트는 tty mode로 존재하는 al의 내용물이다" 라고 알린다. 즉 ah에 넣는단것은 해당 내용물의 당시 상태가 tty mode란것이다.
tty mode는 화면에 한글자씩 출력해주는 모드를 의미한다.

이제 tty mode를 설정, 다시말해 ah에 올릴것인데, 딱 한번만 할것이다. 실제 부트에서 ah에 들어가는 값이 상수라는 보장이라든가 없으니까 말이다. sleep 상태일때 cpu에서 돌아가는 타 프로세스 일부에서는 ah에 쓰레기값을 남기고 청소되지 않은 상태로 두기 때문이다.

하지만 지금은 배우는 단계고, 이해가 우선이니 우리가 넣을값인 이 hello만이 cpu에서 작동중이라 가정하자.

mov ah, 0x0e ; tty mode
mov al, 'H'
int 0x10
mov al, 'e'
int 0x10
mov al, 'l'
int 0x10
int 0x10 ; 'l' is still on al, remember?
mov al, 'o'
int 0x10

jmp $ ; jump to current address = infinite loop

; padding and magic number
times 510 - ($-$$) db 0
dw 0xaa55 

이제 부트 섹터는 Hello 를 출력할것이며, 무한루프에 따라 무한히 기다리는 중일것이다.
