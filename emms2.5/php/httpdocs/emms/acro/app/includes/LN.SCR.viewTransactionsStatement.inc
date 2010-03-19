<?php
require_once 'class/roles.php';
require_once 'class/imf.php';

$date_from = $_REQUEST['date_from'] ? $_REQUEST['date_from'] : date('Y-m-d');
$date_to   = $_REQUEST['date_to']   ? $_REQUEST['date_to']   : date('Y-m-d');

$branch = IMF::getBranches();

$disbursement = WEBPAGE::$dbh->getAssoc(sprintf("
          select 
            branch_id,
            sum(amount) disbursement
          from 
            tblTDebits 
          where 
            date      >= '%s' and 
            date      < '%s'
          group by
            branch_id", $date_from, $date_to));

$recovery = WEBPAGE::$dbh->getAssoc(sprintf("
          select
            l.zone_id branch_id,
            sum(p.principal) recovery
          from
            tblPayments p,
            tblLoans l
          where
            p.date    >= '%s'      and
            p.date    < '%s'      and
            l.id       = p.loan_id
          group by
            l.zone_id", $date_from, $date_to));

$write_off = WEBPAGE::$dbh->getAssoc(sprintf("
          select
            l.zone_id branch_id,
            sum(lwo.principal) write_off
          from
            tblLoanWriteOff lwo,
            tblLoans l
          where
            lwo.date   > '%s'      and
            lwo.date  <= '%s'      and
            l.id       = lwo.loan_id
          group by
            l.zone_id", $date_from, $date_to));

$balance_from = WEBPAGE::$dbh->getAssoc(sprintf("
          select
            zone_id branch_id,
            sum(balance) balance_from
          from
            tblRiskPortfolio
          where
            date    = '%s'
          group by
            zone_id", $date_from));


$balance_to = WEBPAGE::$dbh->getAssoc(sprintf("
          select
            zone_id branch_id,
            sum(balance) balance_to
          from
            tblRiskPortfolio
          where
            date    = '%s'
          group by
            zone_id", $date_to));

foreach ($branch as $branch_id => $branch_name)
{
  $data[$branch_id]['branch']       = $branch_name;
  $data[$branch_id]['balance_from'] = number_format($balance_from[$branch_id], 2, '.', ',');
  $data[$branch_id]['disbursement'] = number_format($disbursement[$branch_id], 2, '.', ',');
  $data[$branch_id]['recovery']     = number_format($recovery[$branch_id], 2, '.', ',');
  $data[$branch_id]['write_off']    = number_format($write_off[$branch_id], 2, '.', ',');
  $data[$branch_id]['result']       = number_format($balance_from[$branch_id] + $disbursement[$branch_id] - $recovery[$branch_id] - $write_off[$branch_id], 2, '.', ',');
  $data[$branch_id]['balance_to']   = number_format($balance_to[$branch_id], 2, '.', ',');
  $data[$branch_id]['diff']         = number_format($balance_to[$branch_id] - ($balance_from[$branch_id] + $disbursement[$branch_id] - $recovery[$branch_id] - $write_off[$branch_id]), 2, '.', ',');

  $total['branch']        = '';
  $total['balance_from'] += $balance_from[$branch_id];
  $total['disbursement'] += $disbursement[$branch_id];
  $total['recovery']     += $recovery[$branch_id];
  $total['write_off']    += $write_off[$branch_id];
  $total['result']       += $balance_from[$branch_id] + $disbursement[$branch_id] - $recovery[$branch_id] - $write_off[$branch_id];
  $total['balance_to']   += $balance_to[$branch_id];
  $total['diff']         += $balance_to[$branch_id] - ($balance_from[$branch_id] + $disbursement[$branch_id] - $recovery[$branch_id] - $write_off[$branch_id]);
}

  $total['branch']       = '';
  $total['balance_from'] = number_format($total['balance_from'], 2, '.', ',');
  $total['disbursement'] = number_format($total['disbursement'], 2, '.', ',');
  $total['recovery']     = number_format($total['recovery'], 2, '.', ',');
  $total['write_off']    = number_format($total['write_off'], 2, '.', ',');
  $total['result']       = number_format($total['result'], 2, '.', ',');
  $total['balance_to']   = number_format($total['balance_to'], 2, '.', ',');
  $total['diff']         = number_format($total['diff'], 2, '.', ',');

$data['total'] = $total;

$head = array
        (
          'branch'       => WEBPAGE::$gt['branch'],
          'balance_from' => WEBPAGE::$gt['portfolio_balance'].'<br />'.date_format(date_create($date_from),WEBPAGE::$gt['date_format']),
          'disbursement' => WEBPAGE::$gt['disbursement'].'*',
          'recovery'     => WEBPAGE::$gt['recovery'].'*',
          'write_off'    => WEBPAGE::$gt['loanWriteOff'].'*',
          'result'       => WEBPAGE::$gt['result'],
          'balance_to'   => WEBPAGE::$gt['portfolio_balance'].'<br />'.date_format(date_create($date_to),WEBPAGE::$gt['date_format']),
          'diff'         => WEBPAGE::$gt['diff']
        );

$tpl->setVariable('rpt_label',          WEBPAGE::$gt['LN.SCR.viewTransactionsStatement']);
$tpl->setVariable('rpt_subtitle_label', sprintf("%s: %s", WEBPAGE::$gt['branch'], $branches[$branch]));
$tpl->setVariable('date_range_label',   WEBPAGE::$gt['dateRange']);
$tpl->setVariable('date_range',         sprintf('%s : %s',date_format(date_create($date_from),WEBPAGE::$gt['date_format']),date_format(date_create($date_to),WEBPAGE::$gt['date_format'])));
$tpl->setVariable('chart',              count($data) ? WEBPAGE::printchart($data,$head) : WEBPAGE::$gt['noData']);

$tpl->setVariable('balance_from', WEBPAGE::$gt['portfolio_balance'].'-'.date_format(date_create($date_from),WEBPAGE::$gt['date_format']));
$tpl->setVariable('balance_to',   WEBPAGE::$gt['portfolio_balance'].'<br />'.date_format(date_create($date_from),WEBPAGE::$gt['date_format']));
$tpl->setVariable('disbursement', WEBPAGE::$gt['disbursement']);
$tpl->setVariable('recovery',     WEBPAGE::$gt['recovery']);
$tpl->setVariable('write_off',    WEBPAGE::$gt['loanWriteOff']);
$tpl->setVariable('result',       WEBPAGE::$gt['result']);
$tpl->setVariable('balance_to',   WEBPAGE::$gt['portfolio_balance'].'-'.date_format(date_create($date_to),WEBPAGE::$gt['date_format']));
$tpl->setVariable('diff',         WEBPAGE::$gt['diff']);

$tpl->setVariable('releasedInPeriod',   WEBPAGE::$gt['releasedInPeriod']);

?>