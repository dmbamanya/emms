<?php
$date = $_REQUEST['date'] ? $_REQUEST['date'] : date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
		 select concat(u.first,' ',u.last) as advisor,
		 		sum(rp.balance) as balance,
		 		sum(rp.riskA-rp.riskB) as riskA, concat(round(100*(sum(rp.riskA-rp.riskB)/sum(rp.balance)),2),'%%') as riskAPct,
		 		sum(rp.riskB-rp.riskC) as riskB, concat(round(100*(sum(rp.riskB-rp.riskC)/sum(rp.balance)),2),'%%') as riskBPct,
		 		sum(rp.riskC-rp.riskD) as riskC, concat(round(100*(sum(rp.riskC-rp.riskD)/sum(rp.balance)),2),'%%') as riskCPct,
		 		sum(rp.riskD) as riskD, concat(round(100*(sum(rp.riskD)/sum(rp.balance)),2),'%%') as riskDPct
		 		from tblRiskPortfolio as rp, tblUsers as u where u.id = rp.advisor_id and rp.date = '%s' group by rp.advisor_id", $date));

$total = current(WEBPAGE::$dbh->getAll(sprintf("
		 select '%s' as advisor,
		 		sum(rp.balance) as balance,
		 		sum(rp.riskA-rp.riskB) as riskA, concat(round(100*(sum(rp.riskA-rp.riskB)/sum(rp.balance)),2),'%%') as riskAPct,
		 		sum(rp.riskB-rp.riskC) as riskB, concat(round(100*(sum(rp.riskB-rp.riskC)/sum(rp.balance)),2),'%%') as riskBPct,
		 		sum(rp.riskC-rp.riskD) as riskC, concat(round(100*(sum(rp.riskC-rp.riskD)/sum(rp.balance)),2),'%%') as riskCPct,
		 		sum(rp.riskD) as riskD, concat(round(100*(sum(rp.riskD)/sum(rp.balance)),2),'%%') as riskDPct
		 		from tblRiskPortfolio as rp, tblUsers as u where u.id = rp.advisor_id and rp.date = '%s'", WEBPAGE::$gt['total'],$date)));
$head = array('advisor'=>WEBPAGE::$gt['tblClients.advisor_id'],'balance'=>WEBPAGE::$gt['portfolio_balance'],'riskA'=>sprintf(WEBPAGE::$gt['par.col.header'],'$',WEBPAGE::$conf['app']['risk.days.A'],WEBPAGE::$conf['app']['risk.days.B']),'riskAPct'=>sprintf(WEBPAGE::$gt['par.col.header'],'%',WEBPAGE::$conf['app']['risk.days.A'],WEBPAGE::$conf['app']['risk.days.B']),'riskB'=>sprintf(WEBPAGE::$gt['par.col.header'],'$',WEBPAGE::$conf['app']['risk.days.B'],WEBPAGE::$conf['app']['risk.days.C']),'riskBPct'=>sprintf(WEBPAGE::$gt['par.col.header'],'%',WEBPAGE::$conf['app']['risk.days.B'],WEBPAGE::$conf['app']['risk.days.C']),'riskC'=>sprintf(WEBPAGE::$gt['par.col.header'],'$',WEBPAGE::$conf['app']['risk.days.C'],WEBPAGE::$conf['app']['risk.days.D']),'riskCPct'=>sprintf(WEBPAGE::$gt['par.col.header'],'%',WEBPAGE::$conf['app']['risk.days.C'],WEBPAGE::$conf['app']['risk.days.D']),'riskD'=>sprintf(WEBPAGE::$gt['par.col.header'],'$',WEBPAGE::$conf['app']['risk.days.D'],'...'),'riskDPct'=>sprintf(WEBPAGE::$gt['par.col.header'],'%',WEBPAGE::$conf['app']['risk.days.D'],'...'));

if (count($data)) { $data['total'] = $total; }

$tpl->setVariable('rpt_label', 			WEBPAGE::$gt['riskPortfolio']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.riskByAdvisor']);
$tpl->setVariable('data_date_label', 	WEBPAGE::$gt['date']);
$tpl->setVariable('data_date', 			$date);
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : WEBPAGE::$gt['noData']);
?>

