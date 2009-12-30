<?php
$date = $_REQUEST['date'] ? $_REQUEST['date'] : date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
		 select concat(u.first,' ',u.last) as advisor,
		 		sum(cp.clients) as clients,sum(cp.female) as female,sum(cp.male) as male,sum(cp.client_i) as client_i,sum(cp.client_g) as client_g,sum(cp.client_b) as client_b,sum(cp.group_g) as group_g,sum(cp.group_b) as group_b,sum(cp.group_bg) as group_bg,sum(cp.client_al) as client_al
		 		from tblClientPortfolio as cp, tblUsers as u where u.id = cp.advisor_id and cp.date = '%s' group by cp.advisor_id", $date));

$total = current(WEBPAGE::$dbh->getAll(sprintf("
		 select '%s' as advisor,
		 		sum(cp.clients) as clients,sum(cp.female) as female,sum(cp.male) as male,sum(cp.client_i) as client_i,sum(cp.client_g) as client_g,sum(cp.client_b) as client_b,sum(cp.group_g) as group_g,sum(cp.group_b) as group_b,sum(cp.group_bg) as group_bg,sum(cp.client_al) as client_al
		 		from tblClientPortfolio as cp, tblUsers as u where u.id = cp.advisor_id and cp.date = '%s'", WEBPAGE::$gt['total'],$date)));

if (count($data)) { $data['total'] = $total; }

$head = array('advisor'=>WEBPAGE::$gt['tblClients.advisor_id'],'clients'=>WEBPAGE::$gt['clients'],'female'=>WEBPAGE::$gt['female'],'male'=>WEBPAGE::$gt['male'],'client_i'=>WEBPAGE::$gt['client_i'],'client_g'=>WEBPAGE::$gt['client_g'],'client_b'=>WEBPAGE::$gt['client_b'],'group_g'=>WEBPAGE::$gt['group_g'],'group_b'=>WEBPAGE::$gt['group_b'],'group_bg'=>WEBPAGE::$gt['group_bg'],'client_al' =>WEBPAGE::$gt['activeWithLoan']);

$tpl->setVariable('rpt_label', 			WEBPAGE::$gt['clientPortfolio']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.clientByAdvisor']);
$tpl->setVariable('data_date_label', 	WEBPAGE::$gt['date']);
$tpl->setVariable('data_date', 			$date);
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : WEBPAGE::$gt['noData']);
?>
