<?php
if ($_GET['mode'] == 'total') { $_GET['mode'] = 1; $_GET['1'] = 1; }

switch ($_GET['cycle']) {
  case 'DAY':
    $mrow = WEBPAGE::$dbh->getAll(sprintf('select date as alt, TO_DAYS(date) as date,100*sum(riskA)/sum(balance) as riskA,100*sum(riskB)/sum(balance) as riskB,100*sum(riskC)/sum(balance) as riskC,100*sum(riskD)/sum(balance) as riskD from tblRiskPortfolio where date >= "%s" and date <= "%s" and %s = %s group by date',$_GET['dateFrom'],$_GET['dateTo'],$_GET['mode'],$_GET[$_GET['mode']]));
    break;
  case 'WEEK':
    $mrow = WEBPAGE::$dbh->getAll(sprintf('select date as alt, YEAR(date)+WEEK(date,WEEKDAY("%s")) as date,100*sum(riskA)/sum(balance) as riskA,100*sum(riskB)/sum(balance) as riskB,100*sum(riskC)/sum(balance) as riskC,100*sum(riskD)/sum(balance) as riskD from tblRiskPortfolio where date >= "%s" and date <= "%s" and %s = %s group by date',$_GET['dateFrom'],$_GET['dateFrom'],$_GET['dateTo'],$_GET['mode'],$_GET[$_GET['mode']]));
    break;
  case 'MONTH':
    $mrow = WEBPAGE::$dbh->getAll(sprintf('select date as alt, YEAR(date)+MONTH(date) as date,100*sum(riskA)/sum(balance) as riskA,100*sum(riskB)/sum(balance) as riskB,100*sum(riskC)/sum(balance) as riskC,100*sum(riskD)/sum(balance) as riskD from tblRiskPortfolio where date >= "%s" and date <= "%s" and %s = %s group by date',$_GET['dateFrom'],$_GET['dateTo'],$_GET['mode'],$_GET[$_GET['mode']]));
    break;
  case 'YEAR':
    $mrow = WEBPAGE::$dbh->getAll(sprintf('select date as alt, YEAR(date) as date,100*sum(riskA)/sum(balance) as riskA,100*sum(riskB)/sum(balance) as riskB,100*sum(riskC)/sum(balance) as riskC,100*sum(riskD)/sum(balance) as riskD from tblRiskPortfolio where date >= "%s" and date <= "%s" and %s = %s group by date',$_GET['dateFrom'],$_GET['dateTo'],$_GET['mode'],$_GET[$_GET['mode']]));
    break;
  }

foreach ($mrow as $key=>$val) {
  $data[sprintf('PAR %s',WEBPAGE::$conf['app']['risk.days.A'])][$val['date']] = $val['riskA'];
  $data[sprintf('PAR %s',WEBPAGE::$conf['app']['risk.days.B'])][$val['date']] = $val['riskB'];
  $data[sprintf('PAR %s',WEBPAGE::$conf['app']['risk.days.C'])][$val['date']] = $val['riskC'];
  $data[sprintf('PAR %s',WEBPAGE::$conf['app']['risk.days.D'])][$val['date']] = $val['riskD'];
  $min_date = min($min_date?$min_date:$val['alt'],$val['alt']);
  $max_date = max($max_date?$max_date:$val['alt'],$val['alt']);
  }
//echo '<pre>';
//print_r($mrow);
//echo '</pre>';
//exit;

$options['x-labels']['on'] = 0;
$options['x-labels']['alt'] = sprintf(WEBPAGE::$gt['from_x_to_y'],$min_date,$max_date);
$graph = New Graph('',WEBPAGE::$gt[$_GET['cycle']],sprintf('%s (%s)',WEBPAGE::$gt['RP.GRP.Performance.riskPortfolio'],'%'),$data,$options);		
Graph::printLines($graph);

?>