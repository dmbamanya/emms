<?php
  $date = $_GET['date'];
  $date_check = explode('-',$date);
  $err = false;
  if (!checkdate($date_check[1],$date_check[2],$date_check[0])) { $err = true; } 
  if (date('Y-m-d') < $date) { $err = true; } 
  
  if ($err)
  {
      echo 'Favor de especificar una fecha valida para ejecutar este reporte.';
      exit;
  }

  $data = WEBPAGE::$dbh->getAll(sprintf("
        select
          lsd.id                       'id',
          l.loan_code                  'loan_code',
          u.username                   'officer',
          c.code                       'client_code',
          concat (c.last,', ',c.first) 'client_name',
          lsd.active                   'delivered_date',
          l.kp                         'kp',
          lf.fund                      'fund',
          z.zone                       'branch',
          l.installment                'installment',
          lod.principal                'delinquent_kp',
          lod.hits                     'delinquent_hits',
          lod.delay                    'delay_total',
          ceil(((2*lod.delay)/(lod.hits+1))+((lod.hits-1)/(lod.hits*(lod.hits+1)))*elt(field(lt.payment_frequency,'W','BW','M','Q','SA','A'),'7','14','30','90','180','360')) 'delay'
        from
          (
            view_loan_status_dates lsd,
            tblLoans l,
            tblClients c,
            view_loan_funds lf,
            tblZones z,
            tblUsers u,
            tblLoanTypes lt
          )
        left join
          tblLoansOnDelinquency lod on lod.loan_id = lsd.id and lod.date = '%s'
        where
          coalesce(lsd.active,'9999-12-31') < '%s' and
          coalesce(lsd.cancelled,lsd.legal_in,'9999-12-31') >= '%s' and
          l.id = lsd.id and
          c.id = l.client_id and
          lf.id = lsd.id and
          z.id = l.zone_id and
          u.id = l.advisor_id and
          lt.id = l.loan_type_id",$date,$date,$date));

  $group = WEBPAGE::$dbh->getAssoc(sprintf("
           select
             lsd.id,
             s.name group_name
           from
             view_loan_status_dates lsd,
             (
               select
                 l.id,
                 lm.borrower_id,
                 lm.borrower_type
               from
                 tblLoans l,
                 tblLoansMasterDetails lmd,
                 tblLoansMaster lm
               where
                 lmd.loan_id = l.id and
                 lm.id = lmd.master_id
             ) lmx
           left join
             tblSocieties s on s.id = lmx.borrower_id and lmx.borrower_type = 'B'
           where
             coalesce(lsd.active,'9999-12-31') < '%s' and
             coalesce(lsd.cancelled,lsd.legal_in,'9999-12-31') >= '%s' and
             lmx.id = lsd.id",$date,$date));

  $balance = WEBPAGE::$dbh->getAssoc(sprintf("
             select
               lsd.id,
               l.kp-coalesce(sum(p.principal),0) balance
             from
               (
                 view_loan_status_dates lsd,
                 tblLoans l
               )
             left join
               tblPayments p on p.loan_id = lsd.id and p.date < '%s'
             where
               coalesce(lsd.active,'9999-12-31') < '%s' and
               coalesce(lsd.cancelled,lsd.legal_in,'9999-12-31') >= '%s' and
               l.id = lsd.id
             group by
               lsd.id", $date,$date,$date));

  $head = array
          (
            'loan_code'       => 'Código del préstamo',
            'client_code'     => 'Cédula',
            'client_name'     => 'Apellido y nombre',
            'group_name'      => 'Grupo',
            'officer'         => 'Asesor',
            'delivered_date'  => 'Fecha otorgamiento',
            'kp'              => 'Monto otorgado',
            'balance'         => 'Balance',
            'fund'            => 'Fondo',
            'branch'          => 'Sucursal',
            'installment'     => 'Plazo en dias',
            'delinquent_kp'   => 'Principal atrasado',
            'delinquent_hits' => 'Cuotas atrasadas',
            'delay_total'     => 'Total dias de atraso',
            'delay'           => 'Atraso aprox'
          );

  foreach($data as $key => $val)
  {
    $loan[$val['id']]['loan_code']       = $val['loan_code'];
    $loan[$val['id']]['client_code']     = $val['client_code'];
    $loan[$val['id']]['client_name']     = $val['client_name'];
    $loan[$val['id']]['group_name']      = $group[$val['id']];
    $loan[$val['id']]['officer']         = $val['officer'];
    $loan[$val['id']]['delivered_date']  = $val['delivered_date'];
    $loan[$val['id']]['kp']              = $val['kp'];
    $loan[$val['id']]['balance']         = $balance[$val['id']];
    $loan[$val['id']]['fund']            = $val['fund'];
    $loan[$val['id']]['branch']          = $val['branch'];
    $loan[$val['id']]['installment']     = $val['installment'];
    $loan[$val['id']]['delinquent_kp']   = $val['delinquent_kp'];
    $loan[$val['id']]['delinquent_hits'] = $val['delinquent_hits'];
    $loan[$val['id']]['delay_total']     = $val['delay_total'];
    $loan[$val['id']]['delay']           = $val['delay'];
  }

  $tpl->setVariable('chart', count($loan) ? WEBPAGE::printchart($loan,$head) : WEBPAGE::$gt['noData']);

?>
