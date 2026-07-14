동작하는 block마다의 시작주소는 bp로 base address다. 각 블록의 시작 주소로 고정된 값인데, 스택은 아래로 자라나므로 "가장 최근 동작한 주소" 인 sp는 스택이 자람에따라 점점 주소가 아래쪽으로 잡히게 된다는것도 상상 가능하다.

__asm__
  mov ah, 0x0e ;; tty mode 즉 텔레타이프
  mov bp, 0x8000 ;; 0x7c00에서 대충 멀리 떨어진 주소로 아무거나 적은거다. overwrite를 피하기위함
  mov sp, bp ;; 스텍이 비었다면 sp는 온전히 bp를 가리킬것. 즉 sp = 0x8000일때를 의미함.

  push 'A' ;; sp -= 2 -> sp = 0x7ffe (A는 0x0041 이기 때문임)
  push 'B' ;; 같은 이유로 0x7ffc에 B 저장
  push 'C' ;; 같은 이유로 0x7ffa에 C 저장

  mov al, [0x7ffe] ;; 0x8000 - 2
  int 0x10 ;; A가 나올것. 

  mov al, [0x8000]
  int 0x10 ;; 스택이 자랐으니 이제 쓰레기값이 있을 최초 sp 위치

;; bx라는 16비트로 이뤄진 2바이트 값의 상위 8비트는 bh, 하위 8비트는 bl이라 부른다. ax, ah, al도 같은 관계인데, 예시를 들자면 bx = 0x0043이라면 bh는 0x00, bl은 0x43이며 참고로 이는 C의 아스키 코드다. 

  pop bx ;; 스택의 top 에서 2바이트를 꺼내 bx에 16비트로 전체를 올린다. push/pop 으로 이뤄진다.
  mov al, bl ;; bx에서 bl부분만 al로 옮긴다. 즉 al은 이제 0x43이 되었을것.
  int 0x10 ;; printf C

  pop bx
  mov al, bl
  int 0x10 ;; printf B

  pop bx
  mov al, bl
  int 0x10 ;; printf A

  mov al, [0x8000]
  int 0x10 ;; 스택에서 한번 pop 된건 현재 해당 스택에 쓰레기값이 존재한다는 근거가 된다.
