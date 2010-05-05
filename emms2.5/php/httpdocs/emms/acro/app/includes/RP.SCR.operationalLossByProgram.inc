<?php
$date_from = $_REQUEST['date_from'] ? $_REQUEST['date_from'] : date('Y-m-d');
$date_to   = $_REQUEST['date_to']   ? $_REQUEST['date_to']   : date('Y-m-d');

$data = WEBPAGE::$dbh->getAssoc(sprintf("
          select
            lsh.loan_id,
            lsh.date dateF,
            l.delivered_date dateT,
            (l.kp+l.kat+l.kaf) balance,
            l.rates_r rate,
            l.zone_id,
            l.program_id,
            l.advisor_id,
            lt.borrower_type,
            p.program program
          from
            tblLoanStatusHistory lsh,
            tblLoans l,
            tblLoanTypes lt,
            tblPrograms p
          where
            p.id       = l.program_id   and
            lt.id      = l.loan_type_id and
            l.id       = lsh.loan_id    and
            lsh.status = 'D'            and
            lsh.date  <= '%s'           and
            (l.delivered_date >= '%s' or l.delivered_date = '0000-00-00')", $date_to, $date_from));

foreach($data as $key=>$val)
{
  $val['dateT'] = ($val['dateT'] == '0000-00-00') ? '9999-01-01' : $val['dateT'];
  if ($val['dateF'] >= $val['dateT']) { continue; }
  $dateF = max($date_from, $val['dateF']);
  $dateT = min($date_to,   $val['dateT']);
  $adF = explode('-', $dateF);
  $adT = explode('-', $dateT);

  $days = ((mktime(0,0,0,$adT[1],$adT[2],$adT[0]) - mktime(0,0,0,$adF[1],$adF[2],$adF[0]))/(60*60*24))+1;
  $loss = round($days*(($val['balance']*$val['rate'])/36500),2);

  $ldata[$val['program_id']]['program'] = $val['program'];
  $total['program'] = 'Total';

  $ldata[$val['program_id']]['B'] += ($val['borrower_type']=='B') ? $loss : 0;
  $ldata[$val['program_id']]['I'] += ($val['borrower_type']=='I') ? $loss : 0;
  $ldata[$val['program_id']]['T'] += $loss;

  $total['B'] += ($val['borrower_type']=='B') ? $loss : 0;
  $total['I'] += ($val['borrower_type']=='I') ? $loss : 0;
  $total['T'] += $loss;
}

if (count($ldata)) { $ldata['total'] = $total; }

$head = array
        (
          'program' => WEBPAGE::$gt['tblPrograms.program'],
          'B'       => WEBPAGE::$gt['tblLoanTypes.borrower_type.B'],
          'I'       => WEBPAGE::$gt['tblLoanTypes.borrower_type.I'],
          'T'       => WEBPAGE::$gt['total']
        );

$tpl->setVariable('rpt_label',          WEBPAGE::$gt['operationalLoss']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.operationalLossByProgram']);
$tpl->setVariable('date_range_label',   WEBPAGE::$gt['dateRange']);
$tpl->setVariable('date_range',         sprintf('%s : %s', $date_from, $date_to));

$tpl->setVariable('chart', count($ldata) ? WEBPAGE::printchart($ldata, $head) : WEBPAGE::$gt['noData']);
?>