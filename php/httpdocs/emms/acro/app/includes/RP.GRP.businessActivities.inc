<?php
$mrow1 = WEBPAGE::$dbh->getAll("
				SELECT bt.activity, count( b.id ) AS hits, pz.short_name AS zone
				FROM tblBusiness AS b, tblBusinessTypes AS bt, tblZones AS z, tblClients AS c, tblZones AS pz
				WHERE bt.id = b.type_id AND FIND_IN_SET(c.id,b.client_list) AND z.id = c.zone_id AND pz.id = z.parent_id
				GROUP BY bt.activity, pz.id"); 
$mrow1 = is_array($mrow1) ? $mrow1 : array();

$zones['TOTAL'] = '';
foreach($mrow1 as $i => $row) {
  $data1[$row[activity]][$row[zone]] = $row[hits];
  $data1[$row[activity]][TOTAL] += $row[hits];
  $zones[$row[zone]] = '';
  $activities[$row[activity]] = '';
  }

foreach($zones as $i => $val_i) {
  foreach($activities as $j => $val_j) {
    $data2[WEBPAGE::$gt[sprintf("tblBusinessTypes.activity.%s",$j)]][$i] = $data1[$j][$i] ? $data1[$j][$i] : 0;
//    $data2[$j][$i] = $data1[$j][$i] ? $data1[$j][$i] : 0;
    }
  }
$grafico = New Graph('','','%',$data2);	
Graph::printStdCols($grafico);
?>
