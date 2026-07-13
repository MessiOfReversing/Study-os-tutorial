목표 : 부트 섹터가 저장된곳을 찾아내자.

직설적으로 이야기하자면 BIOS가 우리가 작성한 부트로더 코드를 복사해서 집어넣는 부트섹터의 주소는 정확히 0x7C00이다. ibm pc 개발 시기부터 정해진 약속이라 변치않는 주소라서 그냥 알아두어야 하는데, 우리가 쓰는 어떤 cpu도 0x7C00이라는걸 직접 눈으로 볼겸 0x7C00을 쓰지 않은 경우와 비교해서 한번 느껴보도록 하자.

X를 4가지 방법을 써서 스크린에 출력해보자. 그중 무엇이 실제로 작동되는가를 볼것이다.

boot_sect_memory.asm 파일에 작성하자.
코드는 아래와 같다.
  
the_secret:
    db "X"

이제 네가지 방법을 쓸것인데
1. mov al, the_secret 의 경우
2. mov al, [the_secret]
3. mov al, the_secret + 0x7C00
4. mov al, 2d + 0x7C00, where 2d is the actual position of the 'X' byte in the binary 이다.

__asm__
  mov ah, 0x0e

  mov al, "1"
  int 0x10
  mov al, the_secret
  int 0x10

  mov al, "2"
  int 0x10 ;; 이제와서 설명하기 좀 그렇긴한데 int는 BIOS 내장함수로 interrupt고, 0x10은 printf같은거라고 보면 된다. 0x0e는 글자를 하나씩 옮겨주는 함수. 그냥 출력할때는 0x0e를 ah에 올리며 시작하고, 매번 입력할것을 al에 올린뒤 int 0x10으로 printf 시키는 템플릿이라고 보면 된다.
  mov al, [the_secret]
  int 0x10

  mov al, "3"
  int 0x10
  mov bx, the_secret
  add bx, 0x7c00
  mov al, [bx]
  int 0x10

;; mov al, [the_secret + 0x7c00] 라고 "3"에서 작성하고 싶었겠지만 cpu는 못알아먹는다. 우선 bx에 the_secret을 올리는 the_secret의 기본 주소 적기를 해낸뒤, 그 후에서야 add bx를 통해 미리 bx에 들어가서 주소로 읽혀진 the_secret에 추가적으로 0x7c00을 더해주고, 그걸 al로 옮겨서 (이때 bx는 [bx]로 대괄호로 묶어주자) int 0x10을 해야 완성이다. []는 마치 C언어에서 포인터(*) 같은 기능을 한다. add로 더해서 오프셋 계산이 끝난걸 다시 []를 씌워서 [bx]형태로 레지스터를 이용할 수 있는건 이 bx뿐이다. 다시말해 [al]같은건 있을수가 없는 문법이다. bx는 base register라고 부른다.
  
  mov al, "4"
  int 0x10
  mov al, [0x7c2d]
  int 0x10

;; "3"에서랑은 다르게 처음부터 오프셋 계산하고 그 결과를 al에 바로 올려도 좋다   

  jmp $ ;; 무한루프

  the_secret:
      db "X"

  times 510-($_$$) db 0
  dw 0xaa55

뭐가 다른건지 알아보자.

코드를 컴파일하고 작동시켜본다면 1[2¢3X4X << 이런 느낌의 문자열이 보일것이다. 
1[쓰레기]2[쓰레기]3X4X << 이런 문자열로 보여야한다.

3과 4 뒤에만 X가 뜬것이 보이는가? [나 유로같은 값이 나오는건 컴파일러는 0부터 주소를 적어버리기 때문에 우리가 추가적으로 0x7C00이라고 더해줘야 하는것이다. X를 입력했으니 그만큼 밀려버려서 mov al 0x7C00이라고만 적어서는 안된다는것도 같은 맥락으로 설명 가능하다.

이제 원리를 알았으니 아래와 같은 명령어를 써보도록 해보자
boot_sect_memory_org.asm에다가 [org 0x7c00] 를 입력하면 앞으로는 
the_secret + 0x7C00같이 입력하지 않고 바로 the_secret이라고만 하더라도 X가 뜰것이다.
