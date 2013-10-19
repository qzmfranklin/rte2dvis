      module zdotx2
        use global
        use inputs
        implicit none
        real(kind=byte),dimension(:),allocatable :: Zon1, Zon2
        real(kind=byte),dimension(:,:),allocatable :: Zoff
        complex(kind=byte),dimension(:,:,:),allocatable :: vTbl, uTbl
        contains
        subroutine dot2(vin,vout)
          implicit none
          complex(kind=byte),dimension(:),intent(in)::vin
          complex(kind=byte),dimension(:),intent(out)::vout
          integer(kind=byte),save::counter=0
          integer(kind=byte) :: n, n_p
          do n=1,Ns
            vout((n-1)*Nm+1:n*Nm)=vout((n-1)*Nm+1:n*Nm)+&
                        Zon1(n)*vin((n-1)*Nm+1:n*Nm)+&
                        Zon2(n)*gTbl*vin((n-1)*Nm+1:n*Nm)
            do n_p=1,n-1
              vout((n-1)*Nm+1:n*Nm)=vout((n-1)*Nm+1:n*Nm)+&
                 (Zoff(n_p,n)*&
                 sum(vin((n-1)*Nm+1:n*Nm)*uTbl(:,n_p,n))&
                 )*conjg(vTbl(:,n_p,n))
            end do
            do n_p=n+1,Ns
              vout((n-1)*Nm+1:n*Nm)=vout((n-1)*Nm+1:n*Nm)+&
                 (Zoff(n_p,n)*&
                 sum(vin((n-1)*Nm+1:n*Nm)*uTbl(:,n_p,n))&
                 !dot_product(vin((n-1)*Nm+1:n*Nm),uTbl(:,n_p,n))&
                 )*conjg(vTbl(:,n_p,n))
            end do
          end do
          counter=counter+1
          write(*,*) "dot2(vin,vout): call(",counter,") completed"
        end subroutine dot2
        subroutine zdotx2_end()
          implicit none
          write(*,*) "zdotx2_end() begins"
          deallocate(Zon1)
          deallocate(Zon2)
          deallocate(Zoff)
          deallocate(vTbl)
          deallocate(uTbl)
          call inputs_end()
          write(*,*) "zdotx2_end() ends"
        end subroutine zdotx2_end
        subroutine zdotx2_test()
          implicit none
          write(*,*) "zdotx2_test() begins"
          !write(*,*) "mutTbl=",mutTbl(1:10)
          !write(*,*) "area=",area(1:10)
          !write(*,*) "Zon1=",Zon1(1:10)
          !write(*,*) "Zon2=",Zon2(1:10)
          write(*,*) "Zoff=",Zoff(1:10,1)
          write(*,*) "vTbl=",vTbl(-3:3,2,1)
          write(*,*) "uTbl=",uTbl(-3:3,2,1)
          write(*,*) "zdotx_test() ends"
        end subroutine zdotx2_test
        subroutine zdotx2_init()
          implicit none
          integer(kind=byte) :: m, m_p, n, n_p ! counters
          complex(kind=byte) :: tmp_xx ! used to calculate vTbl
          write(*,*) "zdotx2_init() begins"
          call inputs_init()
          allocate(Zon1(Ns))
          allocate(Zon2(Ns))
          allocate(Zoff(Ns,Ns))
          allocate(vTbl(-Nd:Nd,Ns,Ns))
          allocate(uTbl(-Nd:Nd,Ns,Ns))
          Zon1=2.0*(sqrt(pi)*mutTbl*area**1.5+pi*area)
          Zon2=-2.0*sqrt(pi)*musTbl*area*1.5
          vTbl(0,:,:)=(1.0,0.0) ! set the known values in vTbl
          do n=1,Ns
            Zoff(n,n)=0.0
            vTbl(:,n,n)=(0.0,0.0)
            uTbl(:,n,n)=(0.0,0.0)
            do n_p=n+1,Ns
              Zoff(n_p,n)=area(n)*area(n_p)/r2rR(n_p,n)
              Zoff(n,n_p)=Zoff(n_p,n) 
              ! construct vTbl(:,n_p,n) efficiently
              tmp_xx=exp((0.0,1.0)*r2rPhi(n_p,n))
              do m=1,Nd ! use symmetry properties to accelerate
                vTbl(m,n_p,n)=tmp_xx*vTbl(m-1,n_p,n)
                vTbl(-m,n_p,n)=conjg(vTbl(m,n_p,n))
                vTbl(m,n,n_p)=-tmp_xx*vTbl(m-1,n,n_p)
                vTbl(-m,n,n_p)=conjg(vTbl(m,n,n_p))
              end do !m
            end do !n_p
          end do !n
          do n=1,Ns ! construct uTbl(:,n_p,n) using array operations
            do n_p=1,Ns
              uTbl(:,n_p,n)= mutTbl(n_p)*vTbl(:,n_p,n)&
                            -musTbl(n_p)*gTbl*vTbl(:,n_p,n)
            end do !n_p
          end do !n
          write(*,*) "zdotx2_init() ends"
        end subroutine zdotx2_init
      end module zdotx2
