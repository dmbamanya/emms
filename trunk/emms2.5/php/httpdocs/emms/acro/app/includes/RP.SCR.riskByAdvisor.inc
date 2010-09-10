<?php
$date = $_REQUEST['date'] ? $_REQUEST['date'] : date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
		 select
                   concat(u.first,' ',u.last) advisor,
                   sum(rp.balance) balance,
                   round(sum(rp.balance) - %s*sum(rp.riskA-rp.riskB) - %s*sum(rp.riskB-rp.riskC) - %s*sum(rp.riskC-rp.riskD) - %s*sum(rp.riskD), 2) netbal,
                   sum(rp.riskA-rp.riskB) riskA,
                   concat(round(100*(sum(rp.riskA-rp.riskB)/sum(rp.balance)),2),'%%') riskAPct,
                   round(%s*sum(rp.riskA-rp.riskB), 2) resA,
                   sum(rp.riskB-rp.riskC) riskB,
                   concat(round(100*(sum(rp.riskB-rp.riskC)/sum(rp.balance)),2),'%%') riskBPct,
                   round(%s*sum(rp.riskB-rp.riskC), 2) resB,
                   sum(rp.riskC-rp.riskD) riskC,
                   concat(round(100*(sum(rp.riskC-rp.riskD)/sum(rp.balance)),2),'%%') riskCPct,
                   round(%s*sum(rp.riskC-rp.riskD), 2) resC,
                   sum(rp.riskD) riskD,
                   concat(round(100*(sum(rp.riskD)/sum(rp.balance)),2),'%%') riskDPct,
                   round(%s*sum(rp.riskD), 2) resD
                 from
                   tblRiskPortfolio rp, tblUsers u
                 where
                   u.id = rp.advisor_id and rp.date = '%s'
                 group by
                   rp.advisor_id",
                 WEBPAGE::$conf['app']['risk.days.A.provision'],
                 WEBPAGE::$conf['app']['risk.days.B.provision'],
                 WEBPAGE::$conf['app']['risk.days.C.provision'], 
                 WEBPAGE::$conf['app']['risk.days.D.provision'],
                 WEBPAGE::$conf['app']['risk.days.A.provision'],
                 WEBPAGE::$conf['app']['risk.days.B.provision'], 
                 WEBPAGE::$conf['app']['risk.days.C.provision'], 
                 WEBPAGE::$conf['app']['risk.days.D.provision'],
                 $date));

$total = current(WEBPAGE::$dbh->getAll(sprintf("
		 select
                   '%s' advisor,
                   sum(rp.balance) balance,
                   round(sum(rp.balance) - %s*sum(rp.riskA-rp.riskB) - %s*sum(rp.riskB-rp.riskC) - %s*sum(rp.riskC-rp.riskD) - %s*sum(rp.riskD), 2) netbal,
                   sum(rp.riskA-rp.riskB) riskA,
                   concat(round(100*(sum(rp.riskA-rp.riskB)/sum(rp.balance)),2),'%%') riskAPct,
                   round(%s*sum(rp.riskA-rp.riskB), 2) resA,
                   sum(rp.riskB-rp.riskC) riskB,
                   concat(round(100*(sum(rp.riskB-rp.riskC)/sum(rp.balance)),2),'%%') riskBPct,
                   round(%s*sum(rp.riskB-rp.riskC), 2) resB,
                   sum(rp.riskC-rp.riskD) riskC,
                   concat(round(100*(sum(rp.riskC-rp.riskD)/sum(rp.balance)),2),'%%') riskCPct,
                   round(%s*sum(rp.riskC-rp.riskD), 2) resC,
                   sum(rp.riskD) riskD,
                   concat(round(100*(sum(rp.riskD)/sum(rp.balance)),2),'%%') riskDPct,
                   round(%s*sum(rp.riskD), 2) resD
                 from
                   tblRiskPortfolio rp, tblUsers u
                 where
                   u.id = rp.advisor_id and rp.date = '%s'",
                   WEBPAGE::$gt['total'],
                   WEBPAGE::$conf['app']['risk.days.A.provision'],
                   WEBPAGE::$conf['app']['risk.days.B.provision'],
                   WEBPAGE::$conf['app']['risk.days.C.provision'],
                   WEBPAGE::$conf['app']['risk.days.D.provision'],
                   WEBPAGE::$conf['app']['risk.days.A.provision'],
                   WEBPAGE::$conf['app']['risk.days.B.provision'],
                   WEBPAGE::$conf['app']['risk.days.C.provision'],
                   WEBPAGE::$conf['app']['risk.days.D.provision'],
                   $date)));
$head = array
        (
          'advisor'  => WEBPAGE::$gt['tblClients.advisor_id'],
          'balance'  => WEBPAGE::$gt['portfolio_balance'],
          'netbal'   => WEBPAGE::$gt['net_balance'],
          'riskA'    => sprintf(WEBPAGE::$gt['par.col.header'],'$',WEBPAGE::$conf['app']['risk.days.A'],WEBPAGE::$conf['app']['risk.days.B']),
          'riskAPct' => sprintf(WEBPAGE::$gt['par.col.header'],'%',WEBPAGE::$conf['app']['risk.days.A'],WEBPAGE::$conf['app']['risk.days.B']),
          'resA'     => sprintf(WEBPAGE::$gt['provision.col.header'],'$',WEBPAGE::$conf['app']['risk.days.A'],WEBPAGE::$conf['app']['risk.days.B']),
          'riskB'    => sprintf(WEBPAGE::$gt['par.col.header'],'$',WEBPAGE::$conf['app']['risk.days.B'],WEBPAGE::$conf['app']['risk.days.C']),
          'riskBPct' => sprintf(WEBPAGE::$gt['par.col.header'],'%',WEBPAGE::$conf['app']['risk.days.B'],WEBPAGE::$conf['app']['risk.days.C']),
          'resB'     => sprintf(WEBPAGE::$gt['provision.col.header'],'$',WEBPAGE::$conf['app']['risk.days.B'],WEBPAGE::$conf['app']['risk.days.C']),
          'riskC'    => sprintf(WEBPAGE::$gt['par.col.header'],'$',WEBPAGE::$conf['app']['risk.days.C'],WEBPAGE::$conf['app']['risk.days.D']),
          'riskCPct' => sprintf(WEBPAGE::$gt['par.col.header'],'%',WEBPAGE::$conf['app']['risk.days.C'],WEBPAGE::$conf['app']['risk.days.D']),
          'resC'     => sprintf(WEBPAGE::$gt['provision.col.header'],'$',WEBPAGE::$conf['app']['risk.days.C'],WEBPAGE::$conf['app']['risk.days.D']),
          'riskD'    => sprintf(WEBPAGE::$gt['par.col.header'],'$',WEBPAGE::$conf['app']['risk.days.D'],'...'),
          'riskDPct' => sprintf(WEBPAGE::$gt['par.col.header'],'%',WEBPAGE::$conf['app']['risk.days.D'],'...'),
          'resD'     => sprintf(WEBPAGE::$gt['provision.col.header'],'$',WEBPAGE::$conf['app']['risk.days.D'],'...')
        );

if (count($data)) { $data['total'] = $total; }

$tpl->setVariable('rpt_label', 			WEBPAGE::$gt['riskPortfolio']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.riskByAdvisor']);
$tpl->setVariable('data_date_label', 	WEBPAGE::$gt['date']);
$tpl->setVariable('data_date', 			$date);
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : WEBPAGE::$gt['noData']);
?>

