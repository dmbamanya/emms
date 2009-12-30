<?php
$date_from 	= $_REQUEST['date_from'] 	? $_REQUEST['date_from'] 	: date('Y-m-d');
$date_to 	= $_REQUEST['date_to'] 		? $_REQUEST['date_to'] 		: date('Y-m-d');
$tpl->setVariable('dateRangeSelector', 	WEBPAGE::$gt['BS.SCR.dateRangeSelector']);
$tpl->setVariable('oldrange', 			sprintf('%s - %s',$date_from,$date_to));

$data = WEBPAGE::$dbh->getAll(sprintf("
		 select cr.date,z.zone,p.program,f.fund,cr.principal,cr.fees,cr.insurances,cr.interest,cr.penalties,cr.amount,cr.code
           from tblTCredits as cr, tblZones as z, tblPrograms as p, tblFunds f
           where z.id = cr.branch_id and p.id = cr.program_id and f.id = cr.fund_id and cr.date >= '%s' and cr.date <= '%s'
           order by cr.branch_id, cr.program_id", $date_from, $date_to));

$head = array('date'=>WEBPAGE::$gt['date'],'zone'=>WEBPAGE::$gt['tblZones.zone'],'program'=>WEBPAGE::$gt['tblPrograms.program'],'fund'=>WEBPAGE::$gt['tblFunds.fund'],'principal'=>WEBPAGE::$gt['principal'],'fees'=>WEBPAGE::$gt['FEES'],'insurances'=>WEBPAGE::$gt['INS'],'interest'=>WEBPAGE::$gt['interest'],'penalties'=>WEBPAGE::$gt['PN'],'amount'=>WEBPAGE::$gt['total'],'code'=>WEBPAGE::$gt['code']);

foreach($data as $key=>$val) {
  $data[$key]['code'] = sprintf('<a href=\'javascript:openWin("index.popup.php?scr_name=LN.SCR.viewTransactionDetails&transaction_id=%s","%s","menubar=no,scrollbars=yes,resizable=yes,width=700,height=320")\' onClick="">%s</a>',$val['code'],$val['code'],$val['code']);
  }
$tpl->setVariable('rpt_label', 			WEBPAGE::$gt['transactionsCredits']);
//$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['transactionsCredits']);
$tpl->setVariable('date_range_label', 	WEBPAGE::$gt['dateRange']);
$tpl->setVariable('date_range', 		sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : WEBPAGE::$gt['noData']);

?>
