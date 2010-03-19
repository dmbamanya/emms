<?php
$date = $_REQUEST['date'] ? $_REQUEST['date'] : date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
		 select z.zone as zone,
		 		count(lod.id) as loans, sum(lod.hits) as hits, sum(lod.delay) as delay,
		 		sum(lod.pmt) as pmt, sum(lod.penalties) as penalties, sum(lod.interest) as interest, sum(lod.fees) as fees, sum(lod.insurances) as insurances,  sum(lod.principal) as principal
		 		from tblLoansOnDelinquency as lod, tblZones as z, tblLoans as l where l.id = lod.loan_id and z.id = l.zone_id and lod.date = '%s' group by l.zone_id", $date));
$total = current(WEBPAGE::$dbh->getAll(sprintf("
		 select '%s' as zone,
		 		count(lod.id) as loans, sum(lod.hits) as hits, sum(lod.delay) as delay,
		 		sum(lod.pmt) as pmt, sum(lod.penalties) as penalties, sum(lod.interest) as interest, sum(lod.fees) as fees, sum(lod.insurances) as insurances,  sum(lod.principal) as principal
		 		from tblLoansOnDelinquency as lod where lod.date = '%s'", WEBPAGE::$gt['total'],$date)));
$head = array('zone'=>WEBPAGE::$gt['tblZones.zone'],'loans'=>WEBPAGE::$gt['loans'],'hits'=>WEBPAGE::$gt['payments'],'delay'=>WEBPAGE::$gt['delay'],'pmt'=>WEBPAGE::$gt['kp'],'penalties'=>WEBPAGE::$gt['PN'],'interest'=>WEBPAGE::$gt['RT'],'fees'=>WEBPAGE::$gt['FEES'],'insurances'=>WEBPAGE::$gt['INS'],'principal'=>WEBPAGE::$gt['KP']);

if(count($data)) { $data['total'] = $total; }

$tpl->setVariable('rpt_label', 			WEBPAGE::$gt['delinquencyPortfolio']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.delinquencyByBranch']);
$tpl->setVariable('data_date_label', 	WEBPAGE::$gt['date']);
$tpl->setVariable('data_date', 			$date);
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : WEBPAGE::$gt['noData']);
?>
