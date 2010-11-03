<?php
$date_from = $_REQUEST['date_from'] ? $_REQUEST['date_from'] : date('Y-m-d');
$date_to   = $_REQUEST['date_to']   ? $_REQUEST['date_to']   : date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
          select
            lwoxcp.*,
            lwoxcp.w_off_total + lwoxcp.w_off_charges - lwoxcp.w_off_payments current_balance,
            coalesce(sum(lwop.amount),0.00) m_pmt
          from
          (
            select
              lwoxc.*,
              coalesce(sum(lwop.amount),0.00) w_off_payments
            from
            (
              select
                lwox.*,
                coalesce(sum(lwoc.amount),0.00) w_off_charges
              from
              (
                select
                  lwo.loan_id                loan_id,
                  l.loan_code                loan_code,
                  concat(c.first,' ',c.last) client,
                  c.code                     client_code,
                  z.short_name               branch,
                  u.username                 w_off_by,
                  lwo.date                   w_off_date,
                  lwo.principal              w_off_principal,
                  lwo.insurance              w_off_insurance,
                  lwo.fees                   w_off_fees,
                  lwo.interest               w_off_interest,
                  lwo.penalties              w_off_penalties,
                  lwo.amount                 w_off_total
                from
                (
                  tblLoanWriteOff lwo,
                  tblLoans        l,
                  tblClients      c,
                  tblZones        z,
                  tblUsers u
                )
                left join
                  tblLoanStatusHistory lsh on lsh.loan_id = lwo.loan_id and lsh.p_status = 'LI' and lsh.date < '%s'
                where
                  lwo.date <= '%s'   and
                  lsh.id is null     and
                  l.id = lwo.loan_id and
                  c.id = l.client_id and
                  z.id = l.zone_id   and
                  u.id = lwo.user_id
              ) lwox
              left join
                tblLoanWriteOffCharges lwoc on lwoc.loan_id = lwox.loan_id
              group by
                lwox.loan_id
            ) lwoxc
            left join
              tblLoanWriteOffPayments lwop on lwop.loan_id = lwoxc.loan_id
            group by
              lwoxc.loan_id
          ) lwoxcp
          left join
            tblLoanWriteOffPayments lwop on lwop.loan_id = lwoxcp.loan_id and lwop.date >= '%s' and lwop.date <= '%s'
          group by
            lwoxcp.loan_id", $date_from, $date_to, $date_from, $date_to));

foreach($data as $key=>$val)
{
  $data[$key]['loan_code'] = sprintf('<a href=\'javascript:openWin("index.popup.php?scr_name=LG.SCR.legalProfile&id=%s","%s","menubar=no,scrollbars=yes,resizable=yes,width=700,height=320")\' onClick="">%s</a>',$val['loan_id'],$val['loan_code'],$val['loan_code']);
}

$head = array
        (
          'loan_code'       => WEBPAGE::$gt['tblLoans.loan_code'],
          'client'          => WEBPAGE::$gt['tblClients.id'],
          'client_code'     => WEBPAGE::$gt['tblClients.code'],
          'branch'          => WEBPAGE::$gt['tblZones.zone'],
          'w_off_by'        => WEBPAGE::$gt['deactivated_by'],
          'w_off_date'      => WEBPAGE::$gt['date'],
          'w_off_principal' => WEBPAGE::$gt['tblPayments.principal'],
          'w_off_insurance' => WEBPAGE::$gt['tblPayments.insurances'],
          'w_off_fees'      => WEBPAGE::$gt['tblPayments.fees'],
          'w_off_interest'  => WEBPAGE::$gt['tblPayments.interest'],
          'w_off_penalties' => WEBPAGE::$gt['tblPayments.penalties'],
          'w_off_total'     => WEBPAGE::$gt['writeOff_total'],
          'w_off_charges'   => WEBPAGE::$gt['legal_charges'],
          'w_off_payments'  => WEBPAGE::$gt['legal_pmt'],
          'current_balance' => WEBPAGE::$gt['current_balance'],
          'm_pmt'           => WEBPAGE::$gt['pmt'].'*'
        );

$pack = array();

$tpl->setVariable('rpt_label',          WEBPAGE::$gt['RP.SCR.loanWriteOff']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.loanWriteOffPayments']);
$tpl->setVariable('date_range_label',   WEBPAGE::$gt['dateRange']);
$tpl->setVariable('date_range',         sprintf('%s : %s',$date_from,$date_to));

$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head,$pack) : WEBPAGE::$gt['noData']);

$tpl->setVariable('recoveryInPeriod',   WEBPAGE::$gt['recoveryInPeriod']);
?>



