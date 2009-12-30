<?php
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';

$client = new CLIENT();

$date_from = $_REQUEST['date_from'] ? $_REQUEST['date_from'] : date('Y-m-d');
$date_to   = $_REQUEST['date_to']   ? $_REQUEST['date_to']   : date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
          select
            l.client_id,
            concat(c.last,', ',c.first) client,
            c.code ccode,
            count(lwo.id) hits,
            sum(lwo.amount) amount,
            sum(lwo.principal) principal,
            sum(lwo.insurance) insurance,
            sum(lwo.fees) fees,
            sum(lwo.interest) interest,
            sum(lwo.penalties) penalties,
            coalesce(sum(lwoc.amount),0.00) lcharges,
            coalesce(sum(lwop.amount),0.00) lpmt,
            (sum(lwo.amount) + coalesce(sum(lwoc.amount),0.00) - coalesce(sum(lwop.amount),0.00)) balance,
            u.username advisor,
            p.program program,
            z.zone branch
          from
            (tblLoanWriteOff lwo,
            tblLoans l,
            tblClients c,
            tblUsers u,
            tblPrograms p,
            tblZones z,
            tblLoanTypes lt)
          left join
            tblLoanWriteOffCharges lwoc on lwoc.loan_id = l.id
          left join
            tblLoanWriteOffPayments lwop on lwop.loan_id = l.id
          where
            lwo.date         >= '%s'           and
            lwo.date         <= '%s'           and
            lt.id             = l.loan_type_id and
            lt.borrower_type != 'G'            and
            l.id              = lwo.loan_id    and
            c.id              = l.client_id    and
            u.id              = l.advisor_id   and
            p.id              = l.program_id   and
            z.id              = l.zone_id
          group by
            l.client_id,
            u.id,
            p.id,
            z.id
          order by
            l.client_id", $date_from, $date_to));

foreach($data as $key=>$val)
{	
  $client->data['id'] = $val['client_id'];
  $client->checkConfidentiality();
  if ($client->data['confidential'])
  {
    $data[$key]['client'] = $client->data['name'];
    $data[$key]['ccode']  = $client->data['code'];
  }	
    
  $ldata[$key]['client']     = str_replace(' ','&nbsp;',$data[$key]['client']);
  $ldata[$key]['ccode']      = str_replace(' ','&nbsp;',$data[$key]['ccode']);
  $ldata[$key]['hits']       = $val['hits'];
  $ldata[$key]['amount']     = $val['amount'];
  $ldata[$key]['principal']  = $val['principal'];
  $ldata[$key]['insurance']  = $val['insurance'];
  $ldata[$key]['fees']       = $val['fees'];
  $ldata[$key]['interest']   = $val['interest'];
  $ldata[$key]['penalties']  = $val['penalties'];
  $ldata[$key]['lcharges']   = $val['lcharges'];
  $ldata[$key]['lpmt']       = $val['lpmt'];
  $ldata[$key]['balance']    = $val['balance'];
  $ldata[$key]['advisor']    = $val['advisor'];
  $ldata[$key]['program']    = $val['program'];
  $ldata[$key]['branch']     = $val['branch'];

  $total['client']           = WEBPAGE::$gt['total'];
  $total['ccode']            = '';
  $total['hits']            += $val['hits'];
  $total['amount']          += $val['amount'];
  $total['principal']       += $val['principal'];
  $total['insurance']       += $val['insurance'];
  $total['fees']            += $val['fees'];
  $total['interest']        += $val['interest'];
  $total['penalties']       += $val['penalties'];
  $total['lcharges']        += $val['lcharges'];
  $total['lpmt']            += $val['lpmt'];
  $total['balance']         += $val['balance'];
  $total['advisor']          = '';
  $total['program']          = '';
  $total['branch']           = '';
}

if (count($ldata))
{ 
  $ldata['total'] = $total;
}

$head = array
        (
          'client'     => WEBPAGE::$gt['tblClients.id'],
          'ccode'      => WEBPAGE::$gt['tblClients.code'],
          'hits'       => sprintf('%s*',WEBPAGE::$gt['loans']),
          'amount'     => sprintf('%s*',WEBPAGE::$gt['total']),
          'principal'  => sprintf('%s*',WEBPAGE::$gt['KP']),
          'insurance'  => sprintf('%s*',WEBPAGE::$gt['INS']),
          'fees'       => sprintf('%s*',WEBPAGE::$gt['FEES']),
          'interest'   => sprintf('%s*',WEBPAGE::$gt['RT']),
          'penalties'  => sprintf('%s*',WEBPAGE::$gt['PN']),
          'lcharges'   => sprintf('%s**',WEBPAGE::$gt['legal_charges']),
          'lpmt'       => sprintf('%s**',WEBPAGE::$gt['pmt']),
          'balance'    => sprintf('%s',WEBPAGE::$gt['current_balance']),
          'advisor'    => WEBPAGE::$gt['tblClients.advisor_id'],
          'program'    => WEBPAGE::$gt['tblPrograms.program'],
          'branch'     => WEBPAGE::$gt['tblZones.zone']
        );

$tpl->setVariable('rpt_label',          WEBPAGE::$gt['writeOffHistory']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.writeOffHistory']);
$tpl->setVariable('date_range_label', 	WEBPAGE::$gt['dateRange']);
$tpl->setVariable('date_range',         sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart',              count($ldata) ? WEBPAGE::printchart($ldata,$head) : WEBPAGE::$gt['noData']);

$tpl->setVariable('writeOffInPeriod',   WEBPAGE::$gt['writeOffInPeriod']);
$tpl->setVariable('totalHistoryAmount', WEBPAGE::$gt['totalHistoryAmount']);

?>

