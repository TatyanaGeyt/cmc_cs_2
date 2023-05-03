extern exp, printf
section .data
    const1 dq -1.0
    const2 dq 3.0
    const3 dq 2.0
section .text
global f1
global f3
global f2

f1: ; e^(-x) + 3
    push ebp
    mov ebp, esp
    sub esp, 24
    
    finit
    fld qword[ebp + 8]
    fld qword[const1]
    fmulp
    fstp qword[esp]
        call exp
    fld qword[const2]
    faddp
    
    leave
    ret

f2: ; 2x - 2
    push ebp
    mov ebp, esp
    
    finit
    fld qword[const3]
    fld qword[ebp + 8]
    fmulp
    fld qword[const3]
    fsubp
    
    leave
    ret
    
f3: ; 1/x
    push ebp
    mov ebp, esp
    
    finit
    fld1
    fld qword[ebp + 8]
    fdivp
    
    leave
    ret
