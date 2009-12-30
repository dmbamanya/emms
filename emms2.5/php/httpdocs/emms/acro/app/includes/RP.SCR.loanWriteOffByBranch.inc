<?php
$date_from 	= $_REQUEST['date_from'] 	? $_REQUEST['date_from'] 	: date('Y-m-d');
$date_to 	= $_REQUEST['date_to'] 		? $_REQUEST['date_to'] 		: date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
		 select l.zone_id, z.zone, lt.borrower_type as type,
		 		sum(lwo.principal) as amount, count(lwo.id) as loans
		 		from tblLoanWriteOff as lwo, tblLoans as l, tblZones as z, tblLoanTypes as lt where l.id = lwo.loan_id and z.id = l.zone_id and lt.id = l.loan_type_id and lwo.date >= '%s' and lwo.date <= '%s' group by l.zone_id, type", $date_from, $date_to));

foreach($data as $key=>$val) {

  $ldata[$val['zone_id']]['zone'] 	 	= str_replace(' ','&nbsp;',$val['zone']);

  $ldata[$val['zone_id']]['loans'] 		+= $val['loans'];
  $ldata[$val['zone_id']]['amount'] 	+= $val['amount'];
  $ldata[$val['zone_id']]['loans_I'] 	+= ($val['type'] == 'I') ? $val['loans'] 			: 0;
  $ldata[$val['zone_id']]['amount_I'] 	+= ($val['type'] == 'I') ? $val['amount'] 			: 0;
  $ldata[$val['zone_id']]['loans_G'] 	+= ($val['type'] == 'G') ? $val['loans'] 			: 0;
  $ldata[$val['zone_id']]['amount_G'] 	+= ($val['type'] == 'G') ? $val['amount'] 			: 0;
  $ldata[$val['zone_id']]['loans_B'] 	+= ($val['type'] == 'B') ? $val['loans'] 			: 0;
  $ldata[$val['zone_id']]['amount_B'] 	+= ($val['type'] == 'B') ? $val['amount'] 			: 0;

  $total['zone'] 	 					 = WEBPAGE::$gt['total'];

  $total['loans'] 						+= $val['loans'];
  $total['amount'] 						+= $val['amount'];
  $total['loans_I'] 					+= ($val['type'] == 'I') ? $val['loans'] 			: 0;
  $total['amount_I'] 					+= ($val['type'] == 'I') ? $val['amount'] 			: 0;
  $total['loans_G'] 					+= ($val['type'] == 'G') ? $val['loans'] 			: 0;
  $total['amount_G'] 					+= ($val['type'] == 'G') ? $val['amount'] 			: 0;
  $total['loans_B'] 					+= ($val['type'] == 'B') ? $val['loans'] 			: 0;
  $total['amount_B'] 					+= ($val['type'] == 'B') ? $val['amount'] 			: 0;

  }

if (count($ldata)) { $ldata['total'] = $total; }

$head = array('zone'=>WEBPAGE::$gt['tblZones.zone'],'loans'=>sprintf('%s*',WEBPAGE::$gt['loans']),'amount'=>sprintf('%s*',WEBPAGE::$gt['tblLoans.kp']),'loans_I'=>sprintf('%s*',WEBPAGE::$gt['loans_I']),'amount_I'=>sprintf('%s*',WEBPAGE::$gt['kp_I']),'loans_G'=>sprintf('%s*',WEBPAGE::$gt['loans_G']),'amount_G'=>sprintf('%s*',WEBPAGE::$gt['kp_G']),'loans_B'=>sprintf('%s*',WEBPAGE::$gt['loans_B']),'amount_B'=>sprintf('%s*',WEBPAGE::$gt['kp_B']));
$tpl->setVariable('rpt_label', 			WEBPAGE::$gt['loanWriteOff']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.loanWriteOffByBranch']);
$tpl->setVariable('date_range_label', 	WEBPAGE::$gt['dateRange']);
$tpl->setVariable('date_range', 		sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart', count($ldata) ? WEBPAGE::printchart($ldata,$head) : WEBPAGE::$gt['noData']);

$tpl->setVariable('writeOffInPeriod', 	WEBPAGE::$gt['writeOffInPeriod']);

?>

