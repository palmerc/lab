<?php
bool class_cutoff_create(class_id)
{
  if (class_exists(class_id))
    query := 'UPDATE class SET cutoffs=”A=90,B=80,C=70,D=60,F=50” WHERE class_id={class_id}'
    result := mysql(query)
    if (result != success)
      return failure
    return success
  else
    return failure
}
bool class_cutoff_edit(class_id, cutoff, value) // Editing value
  if (class_exists(class_id))
{
    // Query string should contain properly formatted SQL
    results := class_cutoff_get_all(class_id)
    i = 0
    while (category != grep('/{cutoff}=\d/', results[i]))
      i++
    results[i] := cutoff.”=”.value
    cutoffs := implode(“,”, results)
    query := 'UPDATE class SET cutoffs={cutoffs} WHERE class_id={class_id}'
    result := mysql(query)
    if (result != success)
      return failure
    return success
  else
    return failure
}
string class_cutoff_get(class_id, cutoff)
{
  if (class_exists(class_id))
    // Query string should contain properly formatted SQL
    query := 'SELECT cutoffs FROM class WHERE class_id={class_id}'
    result := mysql(query) // returns a string
    result := grep('/,*({cutoff}=\d+),*/', result)
    if (result != success)
      return failure
    return result
  else
    return failure
}
array class_cutoff_get_all(class_id)
{
  // Query string should contain properly formatted SQL
  query := 'SELECT cutoffs FROM class WHERE class_id={class_id}'
  results := mysql(query)
  results := explode(“,”, results)
  if (results != success)
    return failure
  else
    return results
}
?>