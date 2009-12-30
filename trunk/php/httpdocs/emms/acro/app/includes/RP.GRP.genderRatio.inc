<?php
$mrow = WEBPAGE::$dbh->getAll('select c.gender, zp.short_name as branch, count(c.id) as hits from tblClients as c, tblZones as z, tblZones as zp 
    							where c.zone_id > 0 and z.id = c.zone_id and zp.id = z.parent_id group by c.gender, zp.id');

foreach($mrow as $key=>$val) {
  $data[WEBPAGE::$gt[sprintf('tblClients.gender.%s',$val['gender'])]]['TOTAL'] += $val['hits'];	
  $data[WEBPAGE::$gt[sprintf('tblClients.gender.%s',$val['gender'])]][$val['branch']] = $val['hits'];	
  }

$graph = New Graph('',WEBPAGE::$gt['tblUsers.zone_id'],'%',$data);		
Graph::printStdCols($graph);
?>
