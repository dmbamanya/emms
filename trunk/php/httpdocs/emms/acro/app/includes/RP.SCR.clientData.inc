<?
$tpl->setVariable('graph_title', WEBPAGE::$gt['RP.GRP.attrition']);
$tpl->setVariable('graph', sprintf("<img src='index.pub.graph.php?scr_name=RP.GRP.attrition&lang=%s' alt=''>",WEBPAGE::$lang));

$tpl->setVariable('cd_graph_title', WEBPAGE::$gt['RP.GRP.clientDistribution']);
$tpl->setVariable('cd_graph', sprintf("<img src='index.pub.graph.php?scr_name=RP.GRP.clientDistribution&lang=%s' alt=''>",WEBPAGE::$lang));

$tpl->setVariable('gr_graph_title', WEBPAGE::$gt['RP.GRP.genderRatio']);
$tpl->setVariable('gr_graph', sprintf("<img src='index.pub.graph.php?scr_name=RP.GRP.genderRatio&lang=%s' alt=''>",WEBPAGE::$lang));
?>
