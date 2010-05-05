SELECT   a.loan_id 'ID Prestamo'                        ,
         l.loan_code 'Codigo Prestamo'                  ,
         l.installment 'Plazo'                          ,
         l.client_id 'Codigo Cliente'                   ,
         CONCAT(c.last,', ',c.first) 'Nombre Cliente'   ,
         l.kp 'Monto Original'                          ,
         (l.kp - COALESCE(p.kpmt,0.00)) 'Balance'       ,
         COALESCE(lod.principal,0.00) 'Principal Atraso',
         COALESCE(lod.hits,0) 'Cuotas Atrasadas'        ,
         COALESCE(lod.delay,0) 'Total Dias Atraso'      ,
         CEILING(COALESCE((2*COALESCE(lod.delay,0)+(ELT(FIND_IN_SET(lt.payment_frequency,'W,BW,M,Q,SA,A'),'7','14','30','91','182','365'))*COALESCE(lod.hits,0)*(COALESCE(lod.hits,0)-1))/(2*COALESCE(lod.hits,0)),0)) 'Atraso Aprox.'
FROM     tblLoanStatusHistory a
       LEFT JOIN   tblLoanStatusHistory b
       ON (
                     b.loan_id  = a.loan_id
                 AND b.p_status = 'G'
                 AND b.date     <= '2009-06-30'
              )
       LEFT JOIN   tblLoans l
       ON     l.id = a.loan_id
       LEFT JOIN
              (SELECT  loan_id,
                       SUM(principal) kpmt
              FROM     tblPayments
              WHERE    DATE <= '2009-06-30'
              GROUP BY loan_id
              ) p
       ON     p.loan_id = a.loan_id
       LEFT JOIN   tblLoansOnDelinquency lod
       ON (
                     lod.loan_id = a.loan_id
                 AND lod.date    = '2009-06-30'
              )
       LEFT JOIN   tblClients c
       ON     c.id = l.client_id
       LEFT JOIN   tblLoanTypes lt
       ON     lt.id = l.loan_type_id
WHERE    a.status   = 'G'
   AND a.date       <= '2009-06-30'
   AND b.loan_id IS NULL;