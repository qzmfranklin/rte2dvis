module global
  implicit none

  integer(kind=4), parameter  :: byte=8
  integer(kind=4), parameter  :: logical_byte=2
  integer(kind=byte), parameter :: max_file_path_len=160
  integer(kind=byte), parameter :: max_cmd_option_len=20
  integer(kind=byte), parameter :: tmp_file_stream=90, &
  tmp_file_stream1=91, tmp_file_stream2=92, tmp_file_stream3=93
  real(kind=byte), parameter  :: pi=&
  3.14159265358979323846264338328d0
  real(kind=byte), parameter  :: e=&
  2.71828182845904523536028747135d0
  real(kind=byte), parameter  :: euler_gamma=&
  0.577215664901532860606512090082d0

  interface norm ! universal norm for vectors and matrices
      module procedure normmr,normmc,&
                       normvr,normvc
  end interface norm

  contains
    subroutine global_end()
      implicit none
      logical(kind=logical_byte),save::end_stat=.false.
      if (end_stat .eqv. .true.) return
      end_stat=.true.
      write(*,*) "global_end() begins"
      write(*,*) "global_end() ends"
    end subroutine global_end
    subroutine global_init()
      implicit none
      logical(kind=logical_byte),save::init_stat=.false.
      if (init_stat .eqv. .true.) return
      init_stat=.true.
      write(*,*) "global_init() begins"
      write(*,*) "byte=",byte
      write(*,*) "max_file_path_len=",max_file_path_len
      write(*,*) "tmp_file_stream=",tmp_file_stream
      write(*,*) "max_cmd_option_len=",max_cmd_option_len
      write(*,*) "pi=",pi
      write(*,*) "e=",e
      write(*,*) "euler_gamma=",euler_gamma
      write(*,*) "global_init() ends"
    end subroutine global_init

    !real(kind=byte) function normmi(m)
    !implicit none
    !integer(kind=byte),dimension(:,:),intent(in)::m
    !normmi=sqrt(sum(m*m))
    !end function normmi
    real(kind=byte) function normmr(m)
    implicit none
    real(kind=byte),dimension(:,:),intent(in)::m
    normmr=sqrt(sum(m*m))
    end function normmr
    real(kind=byte) function normmc(m)
    implicit none
    complex(kind=byte),dimension(:,:),intent(in)::m
    normmc=sqrt(real(sum(m*conjg(m))))
    end function normmc
    !real(kind=byte) function normvi(v)
    !implicit none
    !integer(kind=byte),dimension(:),intent(in)::v
    !normvi=sqrt(sum(v*v))
    !end function normvi
    real(kind=byte) function normvr(v)
    implicit none
    real(kind=byte),dimension(:),intent(in)::v
    normvr=sqrt(sum(v*v))
    end function normvr
    real(kind=byte) function normvc(v)
    implicit none
    complex(kind=byte),dimension(:),intent(in)::v
    normvc=sqrt(real(sum(v*conjg(v))))
    end function normvc

    ! recursively compute the factorial of an integer
    recursive integer(kind=byte) function factorial(n) result(f)
        implicit none
        integer(kind=byte), intent(in)  ::  n
        if (n==1 .or. n==0) then
            f=1
        else
            f=n*factorial(n-1)
        end if
    end function factorial

    ! recursively compute the general term of Fabonacci series
    recursive integer(kind=byte) function fabonacci(n) result(f)
        implicit none
        integer(kind=byte), intent(in)  ::  n
        if (n==1 .or. n==2) then
            f=1
        else
            f=fabonacci(n-1)+fabonacci(n-2)
        end if
    end function fabonacci
end module global
