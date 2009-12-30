<?
$tpl->setVariable('graph_title', WEBPAGE::$gt['RP.GRP.programImpact']);
$tpl->setVariable('graph', sprintf("<img src='index.pub.graph.php?scr_name=RP.GRP.programImpact&survey_id=%s&lang=%s' alt=''>",WEBPAGE::$conf['app']['survey_id'],WEBPAGE::$lang));
?>
