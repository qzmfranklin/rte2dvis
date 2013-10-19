      module inputs
        use global
        implicit none
        integer(kind=byte) :: Ns, Nn, Nd, Nm, Ng, Np
        character(len=max_file_path_len) :: nodes_file=&
        "/Users/qzmfrank/Codes/RTE2DVolumeIntegralSolver/fortran/msh/&
        square948.dat/nodes.dat"
        character(len=max_file_path_len) :: triangles_file=&
        "/Users/qzmfrank/Codes/RTE2DVolumeIntegralSolver/fortran/msh/&
        square948.dat/triangles.dat"
        real(kind=byte)::phis=0.0 ! incidence angle
        real(kind=byte),dimension(:,:),allocatable::p ! nodes' coords
        integer(kind=byte),dimension(:,:),allocatable::t ! triangles' nodes
        real(kind=byte),dimension(:,:,:),allocatable::pt ! triangles' coords
        real(kind=byte),dimension(:,:),allocatable::cntr ! centers
        real(kind=byte),dimension(:,:,:),allocatable::r2rXY ! r to r, Cartesian
        real(kind=byte),dimension(:,:),allocatable::r2rR, r2rPhi ! r to r, Polar
        real(kind=byte),dimension(:),allocatable::gTbl ! phasef table
        real(kind=byte),dimension(:),allocatable::musTbl,muaTbl,mutTbl
        real(kind=byte),dimension(:),allocatable::area ! triangle areas
      contains
        ! modify the following four funtions to definee the material
        real(kind=byte) function phasef(m)
          implicit none
          integer(kind=byte), intent(in) :: m
          real(kind=byte), parameter :: g=0.5
          phasef = g**abs(m)
        end function phasef
        real(kind=byte) function muafun(x,y)
          implicit none
          real(kind=byte), intent(in) :: x,y
          muafun = log(2.0)
        end function muafun
        real(kind=byte) function musfun(x,y)
          implicit none
          real(kind=byte), intent(in) :: x,y
          musfun = 0.0
        end function musfun
        real(kind=byte) function mutfun(x,y)
          implicit none
          real(kind=byte), intent(in) :: x,y
          mutfun = musfun(x,y) + muafun(x,y)
        end function mutfun
        subroutine plwvin(phis,v)
        ! plane wave incidence vector
        ! need to call inputs_init() first
          implicit none
          complex(kind=byte),dimension(:),intent(out)::v
          real(kind=byte),intent(in)::phis
          complex(kind=byte),dimension(:),allocatable::rng
          integer(kind=byte):: n, m
          allocate(rng(-Nd:Nd))
          rng(m)=(1.0,0.0)
          do m=1,Nd
            rng(m)=exp((0.0,-1.0)*m*phis)
            rng(-m)=conjg(rng(m))
          end do
          do n=1,Ns
            v((n-1)*Nm+1:n*Nm)=area(n)*rng
          end do 
          deallocate(rng)
        end subroutine plwvin
        subroutine inputs_end()
          implicit none
          write(*,*) "inputs_end() begins"
          call global_end()
          deallocate(gTbl)
          deallocate(pt)
          deallocate(cntr)
          deallocate(r2rXY)
          deallocate(r2rR)
          deallocate(r2rPhi)
          deallocate(musTbl)
          deallocate(muaTbl)
          deallocate(mutTbl)
          write(*,*) "inputs_end() ends"
        end subroutine inputs_end
        subroutine inputs_init()
          implicit none
          integer(kind=byte) :: m, n, n_p
          write(*,*) "inputs_init() begins"
          call global_init()
          Nd=100
          Nm=2*Nd+1
          call load_nodes(nodes_file, Np, p)
          call load_triangles(triangles_file, Ns, t)
          Nn=Ns
          Ng=Nn*Nm
          write(*,*) "Nd=",Nd," Ns=",Ns," Nn=",Nn," Nm=",Nm," Ng=",Ng
          allocate(gTbl(-Nd:Nd))
          allocate(pt(2,3,Ns))
          allocate(cntr(2,Ns))
          allocate(r2rXY(2,Ns,Ns))
          allocate(r2rR(Ns,Ns))
          allocate(r2rPhi(Ns,Ns))
          allocate(musTbl(Ns))
          allocate(muaTbl(Ns))
          allocate(mutTbl(Ns))
          allocate(area(Ns))
          do m=-Nd,Nd
            gTbl(m)=phasef(m)
          end do
          do n=1,Ns
            pt(:,1,n)=p(:,t(1,n))
            pt(:,2,n)=p(:,t(2,n))
            pt(:,3,n)=p(:,t(3,n))
            cntr(1,n)=sum(pt(1,:,n))/3.0
            cntr(2,n)=sum(pt(2,:,n))/3.0
            area(n)=tri_area(n)
            musTbl(n)=musfun(cntr(1,n),cntr(2,n))
            muaTbl(n)=muafun(cntr(1,n),cntr(2,n))
            r2rXY(:,n,n)=0.0
            r2rR(n,n)=0.0
            r2rPhi(n,n)=0.0
          end do
          mutTbl=musTbl+muaTbl
          do n=1,Ns
            do n_p=n+1,Ns
                r2rXY(:,n_p,n)=cntr(:,n)-cntr(:,n_p)
                r2rXY(:,n,n_p)=-r2rXY(:,n_p,n)
                r2rR(n_p,n)=sqrt(sum(r2rXY(:,n_p,n)**2))
                r2rR(n,n_p)=r2rR(n_p,n)
                r2rPhi(n_p,n)=atan2(r2rXY(2,n_p,n),r2rXY(1,n_p,n))
                r2rPhi(n,n_p)=r2rPhi(n_p,n)+pi
            end do
          end do

          write(*,*) "inputs_init() ends"
          contains
            real(kind=byte) function tri_area(n)
              integer(kind=byte), intent(in) :: n
              real(kind=byte), dimension(2) :: u, v
              u=pt(:,1,n)-pt(:,2,n)
              v=pt(:,1,n)-pt(:,3,n)
              tri_area=abs(u(1)*v(2)-u(2)*v(1))/2.0
            end function tri_area
        end subroutine inputs_init
        subroutine inputs_test()
          write(*,*) "inputs_test() begins"
          write(*,*) cntr(:,1)
          write(*,*) cntr(:,2)
          write(*,*) r2rXY(:,1,2)
          write(*,*) r2rR(1,2)
          write(*,*) r2rPhi(1,2)
          write(*,*) "inputs_test() ends"
        end subroutine inputs_test
        subroutine load_triangles(filename, Ns, t)
          implicit none
          character(len=max_file_path_len), intent(in) :: filename
          integer(kind=byte), intent(out) :: Ns
          integer(kind=byte),dimension(:,:),allocatable,intent(out)::t
          integer(kind=byte), parameter :: file1=tmp_file_stream ! file stream #
          integer(kind=byte) :: ok ! iostat indicator
          integer(kind=byte) :: i ! counter in loop
          real(kind=byte) :: p1,p2,p3 ! used to read data
          write(*,*) "load_triangles() begins"

          Ns=0
          open(unit=file1, file=filename, status="old", iostat=ok)
          !if(ok==0) write(*,*) "file opened: ",filename
          do
            read(file1,*,iostat=ok) p1, p2, p3
            if(ok==0) then ! read one node
                !if (Ns<11) then
                    !write(*,*) "Ns=",Ns,"p1=",int(p1),"p2=",int(p2)&
                    !,"p3=",int(p3)
                !else if (Ns==11) then
                    !write(*,*) "further output suppressed..."
                !end if
            else if(ok>0) then ! system error
                write(*,*) "unexpected error reading: ",filename
                exit
            else ! end of file
                !write(*,*) "reached end of file: ",filename
                write(*,*) "number of triangles=",Ns
                exit
            end if
            Ns=Ns+1
          end do
          close(unit=file1, iostat=ok)
          !if(ok==0) write(*,*) "file closed: ",filename

          allocate(t(3,Ns))    ! allocate memory for nodes
          open(unit=file1, file=filename, status="old", iostat=ok)
          !if(ok==0) write(*,*) "file opened: ",filename
          do i=1,Ns      ! read nodes
            read(file1,*) p1, p2, p3
            t(1,i)=int(p1)
            t(2,i)=int(p2)
            t(3,i)=int(p3)
          end do
          close(unit=file1,iostat=ok)
          !if(ok==0) write(*,*) "triangles successfully loaded: ",filename
          write(*,*) "load_triangles() ends"
        end subroutine load_triangles
        subroutine load_nodes(filename, Np, p)
          implicit none
          character(len=max_file_path_len), intent(in) :: filename
          integer(kind=byte), intent(out) :: Np
          real(kind=byte),dimension(:,:),allocatable,intent(out)::p
          integer(kind=byte), parameter :: file1=tmp_file_stream ! file stream #
          integer(kind=byte) :: ok ! iostat indicator
          integer(kind=byte) :: i ! counter in loop
          real(kind=byte) :: x,y ! used to read data
          write(*,*) "load_nodes() begins"

          Np=0
          open(unit=file1, file=filename, status="old", iostat=ok)
          !if(ok==0) write(*,*) "file opened: ",filename
          do
            read(file1,*,iostat=ok) x, y
            if(ok==0) then ! read one node
                !if (Np<11) then
                    !write(*,*) "Np= ",Np," x= ",x," y= ",y
                !else if (Np==11) then
                    !write(*,*) "further output suppressed..."
                !end if
            else if(ok>0) then ! system error
                write(*,*) "unexpected error reading: ",filename
                exit
            else ! end of file
                !write(*,*) "reached end of file: ",filename
                write(*,*) "number of nodes=",Np
                exit
            end if
            Np=Np+1
          end do
          close(unit=file1, iostat=ok)
          !if(ok==0) write(*,*) "file closed: ",filename

          allocate(p(2,Np))    ! allocate memory for nodes
          open(unit=file1, file=filename, status="old", iostat=ok)
          !if(ok==0) write(*,*) "file opened: ",filename
          do i=1,Np      ! read nodes
            read(file1,*) p(1,i), p(2,i)
          end do
          close(unit=file1,iostat=ok)
          !if(ok==0) write(*,*) "nodes successfully loaded: ",filename
          write(*,*) "load_nodes() ends"
        end subroutine load_nodes
      end module inputs
