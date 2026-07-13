동작하는 block마다의 시작주소는 bp로 base address다. 각 블록의 시작 주소로 고정된 값인데, 스택은 아래로 자라나므로 "가장 최근 동작한 주소" 인 sp는 스택이 자람에따라 점점 주소가 아래쪽으로 잡히게 된다는것도 상상 가능하다.

스택이 자라난다는게 무슨 의미일까 

__asm__
  mov ah, 0x0e ;; tty mode
  mov bp, 0x8000 ;; 0x7c00에서 대충 멀리 떨어진 주소로 아무거나 적은거다. overwrite를 피하기위함
  mov sp, bp ;; 스텍이 비었다면 sp는 온전히 bp를 가리킬것.

  push 'A'
  push 'B'
  push 'C'

  mov al, [0x7ffe] ;; 0x8000 - 2
  int 0x10

  mov al, [0x8000]
  int 0x10

  
