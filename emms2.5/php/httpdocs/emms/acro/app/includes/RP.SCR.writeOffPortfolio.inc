<?php
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';

$client = new CLIENT();

$date_from = $_REQUEST['date_from'] ? $_REQUEST['date_from'] : date('Y-m-d');
$date_to   = $_REQUEST['date_to']   ? $_REQUEST['date_to']   : date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
	select 
          lwo.loan_id,
          lwo.amount,
          lwo.principal,
          lwo.date,
          l.client_id,
          concat(c.last,', ',c.first) client,
          c.code ccode,
          l.loan_code loan,
          u.username advisor,
          p.program program,
          vlf.fund,
          vlg.group_name lgroup,
          z.zone branch
        from
          tblLoanWriteOff lwo, 
          tblLoans l,
          tblClients c,
          tblUsers u,
          tblPrograms p,
          tblZones z,
          tblLoanTypes lt,
          view_loan_funds vlf,
          view_loan_group vlg
        where
          lwo.date >= '%s' and
          lwo.date <= '%s' and
          lt.id = l.loan_type_id and
          lt.borrower_type != 'G' and
          l.id = lwo.loan_id and
          l.status = 'LI' and
          c.id = l.client_id and
          u.id = l.advisor_id and
          p.id = l.program_id and
          z.id = l.zone_id and
          vlf.id = lwo.loan_id and
          vlg.id = lwo.loan_id
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
   
  $ldata[$key]['client']    = str_replace(' ','&nbsp;',$data[$key]['client']);
  $ldata[$key]['ccode']     = str_replace(' ','&nbsp;',$data[$key]['ccode']);
  $ldata[$key]['lgroup']    = str_replace(' ','&nbsp;',$data[$key]['lgroup']);
  $ldata[$key]['loan'] = sprintf('<a href=\'javascript:openWin("index.popup.php?scr_name=LG.SCR.legalProfile&id=%s","%s","menubar=no,scrollbars=yes,resizable=yes,width=700,height=320")\' onClick="">%s</a>',$val['loan_id'],$val['loan'],$val['loan']);
  $ldata[$key]['date']      = $val['date'];
  $ldata[$key]['amount']    = $val['amount'];
  $ldata[$key]['principal'] = $val['principal'];
  $ldata[$key]['advisor']   = $val['advisor'];
  $ldata[$key]['program']   = $val['program'];
  $ldata[$key]['fund']      = $val['fund'];
  $ldata[$key]['branch']    = $val['branch'];

  $total['client']          = WEBPAGE::$gt['total'];
  $total['ccode']           = '';
  $total['lgroup']          = '';
  $total['loan']            = '';
  $total['date']            = '';
  $total['amount']         += $val['amount'];
  $total['principal']      += $val['principal'];
  $total['advisor']         = '';
  $total['program']         = '';
  $total['fund']            = '';
  $total['branch']          = '';
}

if (count($ldata)) { $ldata['total'] = $total; }

$head = array
        (
          'client'    => WEBPAGE::$gt['tblClients.id'],
          'ccode'     => WEBPAGE::$gt['tblClients.code'],
          'lgroup'    => WEBPAGE::$gt['tblClients.society_id'],
          'loan'      => WEBPAGE::$gt['tblLoans.loan_code'],
          'date'      => WEBPAGE::$gt['date'],
          'amount'    => WEBPAGE::$gt['writeOff_amount'],
          'principal' => WEBPAGE::$gt['principal'],
          'advisor'   => WEBPAGE::$gt['tblClients.advisor_id'],
          'program'   => WEBPAGE::$gt['tblPrograms.program'],
          'fund'      => WEBPAGE::$gt['tblFunds.fund'],
          'branch'    => WEBPAGE::$gt['tblZones.zone']
        );

$tpl->setVariable('rpt_label',          WEBPAGE::$gt['writeOffPortfolio']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.writeOffPortfolio']);
$tpl->setVariable('date_range_label',   WEBPAGE::$gt['dateRange']);
$tpl->setVariable('date_range',         sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart', count($ldata) ? WEBPAGE::printchart($ldata,$head) : WEBPAGE::$gt['noData']);

?>


