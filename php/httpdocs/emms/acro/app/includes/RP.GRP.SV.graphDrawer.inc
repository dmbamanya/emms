<?php
require_once 'class/survey.php';
$data = SURVEY::getGraphData($_GET['survey_id'],$_GET['client_id'],$_GET['lang']);
$grafico = New Graph('','','%',$data);
switch (count($data)) {
  case 0:
    break;
  case 1:
    Graph::printBars($grafico); 
    break;
  default:
    Graph::printBends($grafico); 
  }	
?>