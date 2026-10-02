program adaptative_benchmark

  ! declaraciones
  INTEGER, PARAMETER:: REPEAT = 5
  INTEGER:: k, counter
  ! TMIN TMAX evitar outliers (poco o mucho tiempo)
  REAL:: limit, limit0, speedup, tmin, tmax
  REAL:: time1, time0, time_elapsed
  REAL, DIMENSION(REPEAT):: sample 
  REAL:: ave, var
  LOGICAL:: valid_measurement
  
  REAL:: sum_dummy

  ! config:
  limit0 =  1000000.0
  speedup = 1.0
  tmin = 0.1
  tmax = 0.5

  ! limit: #veces ejecutara el test en una <observacion>
  ! counter: #veces que se ejecuto el test
  limit = limit0 * speedup * (tmax - tmin) / 2.0
  ! limit0: #veces necesarias para ejecutar el test en una maquina de referencia para obtener tiempo entre tmax y tmin
  ! speedup: %de velocidad en comparacion a la maquina de referencia
  ! se toma como limit para esta maquina el limit necesario en proporcion al limit necesario para que benchmark dure tmax - min / 2

  DO k = 1, repeat 
    valid_measurement = .FALSE.

    DO WHILE (.NOT. valid_measurement)
      counter = 1
      sum_dummy = 0.0 

      CALL CPU_TIME(time0)
      
      ! test
      counter = 1
      DO WHILE (counter <= LIMIT)
        sum_dummy = sum_summy + SQRT(REAL(counter))
        counter = counter + 1
      END DO

      CALL CPU_TIME(time1)
      elapsed_time = time1 - time0

      IF (elapsed_time >= tmin .AND. elapsed_time <= tmax) THEN
        valid_measurement = .TRUE.
      ELSE 
        ! el tiempo de ejecucion esta fuera del rango
        ! por el contexto (SO) se exedio 
        ! por que el test() es muy rapido hay que incrementar
        ! la proporcion para actualizar limit(INT) es ok_elapsed / curr_elaps
        limit = limit * 0.5 * (tmax - tmin) / elapsed_time
      END IF

    END DO
    ! almacena muestras aceptables del tiempo de ejecucion del test
    sample(k) = elapsed_time / limit
    print  *, 'Muestra ', k, 'ejecutada en ', elapsed_time, 's. Tiempo/niteraciones ', sample(k)
  END DO

  ave = SUM(sample) / REPEAT
  var = SUM((sample - ave)**2) / REAL(REPEAT - 1)

  PRINT *, '---------------------------------------'
  PRINT *, 'test SQRT(REAL)'
  PRINT *, 'Promedio de duracion del test: ', ave
  PRINT *, 'Varianza de duracion del test: ', var
end program adaptative_benchmark

