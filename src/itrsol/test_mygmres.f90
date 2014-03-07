subroutine GMRES(b,x,n,r,w,r3,error0,max_iter,iter,gmresm,gmres_HH,gmres_VV)
        implicit none
        integer i, j ,ii, jj, iii, jjj 
        integer edge_m, edge_n
        integer n,nnn,kk
        integer gmresm,m,preconditioner,SAI_level,Diag_level
        complex gmres_VV(n,gmresm+1),ym(gmresm+1)
        complex gmres_HH(gmresm+1,gmresm+1)
        complex  b(n),x(n),r(n),w(n),r3(n),ctemp
        complex  s(gmresm+1),sn(gmresm+1)
        real cs(gmresm+1) 
        integer   flag,restart,max_iter,iter
        real   ulo1,ulo,rep,amda,beta,err,error0
        real normb,normw,rel_tol
        real T0
        complex neiji

        m=gmresm
        restart=0
        err=1.0

          do j=1,m+1
           do i=1,n
            gmres_VV(i,j)=(0.0,0.0)
          enddo
        enddo

          do j=1,m+1
           do i=1,m+1
            gmres_HH(i,j)=(0.0,0.0)
          enddo
        enddo

        do i=1,m+1
          ym(i)=(0.0,0.0)
          s(i)=(0.0,0.0)
          sn(i)=(0.0,0.0)
          cs(i)=0.0
        enddo


        normb=0.0
        do i=1,n
            normb=normb+abs(b(i))**2
        enddo
        normb=sqrt(normb)

        ! call MVP(A,x,r) 
        call Fast_matrix_vector_multiply(n,x,r)

        do i=1,n
            r(i)=b(i)-r(i)
        enddo


        if (normb<1e-16) then
            rel_tol=error0*1e-16
        else
            rel_tol=error0*normb
        endif

!	if (normb<1e-16) then
!	  cie=cie*1e-16
!	else
!	  cie=cie*normb
!	endif

        beta=0.0
        do i=1,n
          beta=beta+abs(r(i))**2
        enddo
        beta=sqrt(beta)

        if (beta<=rel_tol) then
          beta=beta/normb
         return
        endif

        err=1.0
        iter=0
        j=1

        open(6,carriagecontrol='fortran')
        open(12,file='GMRES_performence.txt')

        T0=secnds(0.0)

        do while (j<max_iter)

            do i=1,n
            gmres_VV(i,1)=r(i)/beta
            enddo

            do i=1,m+1
            s(i)=(0.0,0.0)
            enddo
            s(1)=beta*(1.0,0.0)

            do i=1,m
            do nnn=1,n
            r3(nnn)=gmres_VV(nnn,i)
            enddo

            !call MVP(A,r3,w)
            call Fast_matrix_vector_multiply(n,r3,w)

            do kk=1,i
            do nnn=1,n
            r3(nnn)=gmres_VV(nnn,kk)
            enddo
            gmres_HH(kk,i)=neiji(r3,w,n)
            do nnn=1,n
            w(nnn)=w(nnn)-gmres_HH(kk,i)*gmres_VV(nnn,kk)
            enddo
            enddo

            normw=0.0
            do nnn=1,n
            normw=normw+abs(w(nnn))**2
            enddo
            normw=sqrt(normw)
            gmres_HH(i+1,i)=normw

            do nnn=1,n
            gmres_VV(nnn,i+1)=w(nnn)/normw
            enddo

            if (i>1) then
                    do kk=1,i-1
                    call ApplyPlaneRotation(gmres_HH(kk,i),gmres_HH(kk+1,i),cs(kk),sn(kk))
                    enddo
            endif

            call GeneratePlaneRotation(gmres_HH(i,i),normw,cs(i),sn(i))
            call ApplyPlaneRotation(gmres_HH(i,i),gmres_HH(i+1,i),cs(i),sn(i))
            call ApplyPlaneRotation(s(i),s(i+1),cs(i),sn(i))

            beta=abs(s(i+1))
            iter = i+restart*gmresm

            err=beta/normb
            write(6,FMT='(1H+,A,I,A,F,F,A,A1)') 'ITER:',iter,'         RSS:',err,secnds(T0)/real(iter),'s per iteration',char(0)
            write(12,*) ITER,err
            !write(*,*)'iter=',iter,'beta=',beta/normb

            !open(108, file='out_sys.txt', access='append')
            !write(108,*)  iter,beta/normb
            !close(108)

            if (beta<=rel_tol) then
                    write(*,*)"   "
                    write(*,*) 'convergence achived.'
                    write(*,'(F,A)') secnds(T0),"s costed"
                    write(*,*)"   "
                    call update(x,i-1,gmres_HH,s,gmres_VV,n,m)
                    goto 1111
            endif
            j=j+1
            enddo

            call update(x,gmresm-1,gmres_HH,s,gmres_VV,n,m)

            !call MVP(A,x,r)
            call Fast_matrix_vector_multiply(n,x,r)

            do nnn=1,n
            r(nnn)=b(nnn)-r(nnn)
            enddo


            beta=0.0
            do nnn=1,n
            beta=beta+abs(r(nnn))**2
            enddo
            beta=sqrt(beta)

            restart=restart+1
            iter = restart*gmresm

            err=beta/normb
            write(6,FMT='(1H+,A,I,A,F,F,A,A1)') 'ITER:',iter,'         RSS:',err,secnds(T0)/real(iter),'s per iteration',char(0)
            write(12,*) ITER,err
            !write(*,*)'iter=',iter,'beta=',beta/normb
            !open(108, file='out_sys.txt', access='append')
            !write(108,*) iter,beta/normb
            !close(108)

            if(beta<rel_tol) then
                    write(*,*)"   "
                    write(*,*) 'convergence achived.'
                    write(*,'(F,A)') secnds(T0),"s costed"
                         write(*,*)"   "
                         goto 1111
            endif

    enddo


        !1111  write(*,*)'beta=',rel_tol
        write(*,*)"   "
        write(*,*) 'itmax exceeded;no convergence.'
        write(*,'(F,A)') secnds(T0),"s costed"
        write(*,*)"   "

        1111 continue
        close(6)
        close(12) 
        beta=beta/normb

        return

end subroutine GMRES

subroutine update(x,k,h,s,v,n,m)

        integer k,n,m,i
        complex y(k+1),x(n),h(m+1,m+1),v(n,m+1),s(m+1)

        do i=1,k+1
          y(i)=s(i)
        enddo

    do i=k+1,1,-1
            y(i)=y(i)/h(i,i)
            if (i>1) then
                    do j=i-1,1,-1
                    y(j)=y(j)-h(j,i)*y(i)
                    enddo
            endif
        enddo

        do j=1,k+1
            do i=1,n
            x(i)=x(i)+v(i,j)*y(j)
            enddo
        enddo

        return

end subroutine update

subroutine GeneratePlaneRotation(x,y,cs,sn)

        complex x,sn
        real y,cs,ynorm,rel

        if (y==0) then 
          cs=1.0
          sn=(0.0,0.0)
        elseif (x==(0.0,0.0)) then
          cs=0.0
          sn=(1.0,0.0)
        else
          rel=abs(x/y)
          ynorm=1.0/sqrt(1.0+rel*rel)
          cs=rel*ynorm
          sn=cs*y/x
        endif

        return

end subroutine GeneratePlaneRotation

subroutine ApplyPlaneRotation(x,y,cs,sn)

        complex x,y,sn,temp
        real cs

        temp=cs*x+conjg(sn)*y
        y=cs*y-sn*x
        x=temp

        return

end subroutine ApplyPlaneRotation

function neiji(a,b,n)

        complex neiji
        integer n,i
        complex a(n),b(n)
        complex ret
        ret=(0.0,0.0)
        do i=1,n
          ret=ret+conjg(a(i))*b(i)
        enddo
        neiji=ret

        return

end function neiji


