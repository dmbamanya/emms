<?php
$data[0] = WEBPAGE::$dbh->getAssoc('select zp.short_name, count(c.id) from tblClients as c, tblZones as z, tblZones as zp 
    							where c.zone_id > 0 and z.id = c.zone_id and zp.id = z.parent_id group by zp.id');

$graph = New Graph('',WEBPAGE::$gt['tblUsers.zone_id'],WEBPAGE::$gt['active_associates'],$data);		
Graph::printBars($graph); 
?>
