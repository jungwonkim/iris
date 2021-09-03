MODULE BRISBANE
    USE, INTRINSIC :: ISO_C_BINDING
    IMPLICIT NONE

    INTEGER, PARAMETER :: BRISBANE_OK       = 0
    INTEGER, PARAMETER :: BRISBANE_ERR      = -1

    INTEGER, PARAMETER :: BRISBANE_CPU      = LSHIFT(1, 6)
    INTEGER, PARAMETER :: BRISBANE_NVIDIA   = LSHIFT(1, 7)
    INTEGER, PARAMETER :: BRISBANE_AMD      = LSHIFT(1, 8)
    INTEGER, PARAMETER :: BRISBANE_GPU      = BRISBANE_NVIDIA + BRISBANE_AMD

    INTEGER, PARAMETER :: BRISBANE_R        = -1
    INTEGER, PARAMETER :: BRISBANE_W        = -2
    INTEGER, PARAMETER :: BRISBANE_RW       = -3

    INTERFACE

    INTEGER(C_INT) FUNCTION BRISBANE_INIT_CBIND(ARGC, ARGV, SYNC) &
        BIND(C, NAME='brisbane_init')
        USE, INTRINSIC :: ISO_C_BINDING
        INTEGER(C_INT), VALUE, INTENT(IN) :: ARGC
        TYPE(C_PTR), VALUE, INTENT(IN) :: ARGV
        INTEGER(C_INT), VALUE, INTENT(IN) :: SYNC
    END FUNCTION BRISBANE_INIT_CBIND

    INTEGER(C_INT) FUNCTION BRISBANE_FINALIZE_CBIND() &
        BIND(C, NAME='brisbane_finalize')
        USE, INTRINSIC :: ISO_C_BINDING
    END FUNCTION BRISBANE_FINALIZE_CBIND

    INTEGER(C_INT) FUNCTION BRISBANE_MEM_CREATE_CBIND(SIZE, MEM) &
        BIND(C, NAME='brisbane_mem_create')
        USE, INTRINSIC :: ISO_C_BINDING
        INTEGER(C_SIZE_T), VALUE, INTENT(IN) :: SIZE
        INTEGER(C_SIZE_T), INTENT(OUT) :: MEM
    END FUNCTION BRISBANE_MEM_CREATE_CBIND

    INTEGER(C_INT) FUNCTION BRISBANE_TASK_CREATE_CBIND(TASK) &
        BIND(C, NAME='brisbane_task_create')
        USE, INTRINSIC :: ISO_C_BINDING
        INTEGER(C_SIZE_T), INTENT(OUT) :: TASK
    END FUNCTION BRISBANE_TASK_CREATE_CBIND

    INTEGER(C_INT) FUNCTION BRISBANE_TASK_H2D_FULL_CBIND(TASK, MEM, HOST) &
        BIND(C, NAME='brisbane_task_h2d_full')
        USE, INTRINSIC :: ISO_C_BINDING
        INTEGER(C_SIZE_T), VALUE, INTENT(IN) :: TASK
        INTEGER(C_SIZE_T), VALUE, INTENT(IN) :: MEM
        !TYPE(C_PTR), VALUE, INTENT(IN) :: HOST
        REAL(C_FLOAT), DIMENSION(*), INTENT(IN) :: HOST
    END FUNCTION BRISBANE_TASK_H2D_FULL_CBIND

    INTEGER(C_INT) FUNCTION BRISBANE_TASK_D2H_FULL_CBIND(TASK, MEM, HOST) &
        BIND(C, NAME='brisbane_task_d2h_full')
        USE, INTRINSIC :: ISO_C_BINDING
        INTEGER(C_SIZE_T), VALUE, INTENT(IN) :: TASK
        INTEGER(C_SIZE_T), VALUE, INTENT(IN) :: MEM
        !TYPE(C_PTR), VALUE, INTENT(IN) :: HOST
        REAL(C_FLOAT), DIMENSION(*), INTENT(IN) :: HOST
    END FUNCTION BRISBANE_TASK_D2H_FULL_CBIND

    INTEGER(C_INT) FUNCTION BRISBANE_TASK_KERNEL_CBIND(TASK, KERNEL, DIM, OFF, &
      GWS, LWS, NPARAMS, PARAMS, PARAMS_INFO) &
        BIND(C, NAME='brisbane_task_kernel')
        USE, INTRINSIC :: ISO_C_BINDING
        INTEGER(C_SIZE_T), VALUE, INTENT(IN) :: TASK
        CHARACTER, DIMENSION(*), INTENT(IN) :: KERNEL
        INTEGER(C_INT), VALUE, INTENT(IN) :: DIM
        INTEGER(C_SIZE_T), DIMENSION(*), INTENT(IN) :: OFF
        INTEGER(C_SIZE_T), DIMENSION(*), INTENT(IN) :: GWS
        INTEGER(C_SIZE_T), DIMENSION(*), INTENT(IN) :: LWS
        INTEGER(C_INT), VALUE, INTENT(IN) :: NPARAMS
        INTEGER(C_SIZE_T), DIMENSION(*), INTENT(IN) :: PARAMS
        INTEGER(C_INT), DIMENSION(*), INTENT(IN) :: PARAMS_INFO
    END FUNCTION BRISBANE_TASK_KERNEL_CBIND

    INTEGER(C_INT) FUNCTION BRISBANE_TASK_SUBMIT_CBIND(TASK, DEV, OPT, SYNC) &
        BIND(C, NAME='brisbane_task_submit')
        USE, INTRINSIC :: ISO_C_BINDING
        INTEGER(C_SIZE_T), VALUE, INTENT(IN) :: TASK
        INTEGER(C_INT), VALUE, INTENT(IN) :: DEV
        TYPE(C_PTR), VALUE, INTENT(IN) :: OPT
        INTEGER(C_INT), VALUE, INTENT(IN) :: SYNC
    END FUNCTION BRISBANE_TASK_SUBMIT_CBIND

    END INTERFACE

    CONTAINS

    SUBROUTINE BRISBANE_INIT(SYNC, IERROR)
        LOGICAL, INTENT(IN) :: SYNC
        INTEGER, INTENT(OUT) :: IERROR
        INTEGER(C_INT) :: SYNC_C
        IF (SYNC) THEN
          SYNC_C = 1
        ELSE
          SYNC_C = 0
        END IF
        IERROR = BRISBANE_INIT_CBIND(0, C_NULL_PTR, SYNC_C)
    END SUBROUTINE BRISBANE_INIT

    SUBROUTINE BRISBANE_FINALIZE(IERROR)
        INTEGER, INTENT(OUT) :: IERROR
        IERROR = BRISBANE_FINALIZE_CBIND()
    END SUBROUTINE BRISBANE_FINALIZE

    SUBROUTINE BRISBANE_MEM_CREATE(SIZE, MEM, IERROR)
        INTEGER(8), INTENT(IN) :: SIZE
        INTEGER(8), INTENT(OUT) :: MEM
        INTEGER, INTENT(OUT) :: IERROR
        IERROR = BRISBANE_MEM_CREATE_CBIND(SIZE, MEM)
    END SUBROUTINE BRISBANE_MEM_CREATE

    SUBROUTINE BRISBANE_TASK_CREATE(TASK, IERROR)
        INTEGER(8), INTENT(OUT) :: TASK 
        INTEGER, INTENT(OUT) :: IERROR
        IERROR = BRISBANE_TASK_CREATE_CBIND(TASK)
    END SUBROUTINE BRISBANE_TASK_CREATE

    SUBROUTINE BRISBANE_TASK_H2D_FULL(TASK, MEM, HOST, IERROR)
        INTEGER(8), INTENT(IN) :: TASK 
        INTEGER(8), INTENT(IN) :: MEM
        REAL, DIMENSION(*), INTENT(IN) :: HOST
        INTEGER, INTENT(OUT) :: IERROR
        IERROR = BRISBANE_TASK_H2D_FULL_CBIND(TASK, MEM, HOST)
    END SUBROUTINE BRISBANE_TASK_H2D_FULL

    SUBROUTINE BRISBANE_TASK_D2H_FULL(TASK, MEM, HOST, IERROR)
        INTEGER(8), INTENT(IN) :: TASK 
        INTEGER(8), INTENT(IN) :: MEM
        REAL, DIMENSION(*), INTENT(IN) :: HOST
        INTEGER, INTENT(OUT) :: IERROR
        IERROR = BRISBANE_TASK_D2H_FULL_CBIND(TASK, MEM, HOST)
    END SUBROUTINE BRISBANE_TASK_D2H_FULL

    SUBROUTINE BRISBANE_TASK_KERNEL(TASK, KERNEL, DIM, OFF, &
      GWS, LWS, NPARAMS, PARAMS, PARAMS_INFO, IERROR)
        INTEGER(8), INTENT(IN) :: TASK
        CHARACTER, DIMENSION(*), INTENT(IN) :: KERNEL
        INTEGER, INTENT(IN) :: DIM
        INTEGER(8), DIMENSION(*), INTENT(IN) :: OFF
        INTEGER(8), DIMENSION(*), INTENT(IN) :: GWS
        INTEGER(8), DIMENSION(*), INTENT(IN) :: LWS
        INTEGER, INTENT(IN) :: NPARAMS
        INTEGER(8), DIMENSION(*), TARGET, INTENT(IN) :: PARAMS
        INTEGER, DIMENSION(*), INTENT(IN) :: PARAMS_INFO
        INTEGER, INTENT(OUT) :: IERROR

        INTEGER(8),DIMENSION(NPARAMS) :: NEW_PARAMS
        INTEGER :: PI
        INTEGER :: I
        DO I = 1, NPARAMS
          PI = PARAMS_INFO(I)
          IF (PI == BRISBANE_RW .OR. PI == BRISBANE_R .OR. &
              PI == BRISBANE_W) THEN
            NEW_PARAMS(I) = PARAMS(I)
          ELSE
            NEW_PARAMS(I) = TRANSFER(C_LOC(PARAMS(I)), TASK)
          ENDIF
        ENDDO

        IERROR = BRISBANE_TASK_KERNEL_CBIND(TASK, KERNEL, DIM, OFF, &
          GWS, LWS, NPARAMS, NEW_PARAMS, PARAMS_INFO)
    END SUBROUTINE BRISBANE_TASK_KERNEL

    SUBROUTINE BRISBANE_TASK_SUBMIT(TASK, DEV, SYNC, IERROR)
        INTEGER(8), INTENT(IN) :: TASK 
        INTEGER, INTENT(IN) :: DEV
        LOGICAL, INTENT(IN) :: SYNC
        INTEGER, INTENT(OUT) :: IERROR
        INTEGER :: SYNC_C
        IF (SYNC) THEN
          SYNC_C = 1
        ELSE
          SYNC_C = 0
        END IF
        IERROR = BRISBANE_TASK_SUBMIT_CBIND(TASK, DEV, C_NULL_PTR, SYNC_C)
    END SUBROUTINE BRISBANE_TASK_SUBMIT

END MODULE BRISBANE

