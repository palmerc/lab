<?php
bool class_category_create(class_id, category, value, rank)
{
  if (!class_category_exists(class_id, category))
    // Query string should contain properly formatted SQL
    results := class_category_get_all(class_id)
    new_array[] := array_merge(array_slice(results, 0, rank),array({category}.'='.{value}),array_slice(results, rank))
    categories := implode(“,”, new_array)
    query := 'UPDATE class SET categories={categories} WHERE class_id={class_id}'
    result := mysql(query)
    if (result != success)
      return failure
    return success
  else
    return failure
}
bool class_category_edit(class_id, category, value) // Editing value
{
  if (class_category_exists(class_id, category))
    // Query string should contain properly formatted SQL
    results := class_category_get_all(class_id)
    i = 0
    while (category != grep('/{category}=\d/', results[i]))
      i++
    results[i] := category.”=”.value
    categories := implode(“,”, results)
    query := 'UPDATE class SET categories={categories} WHERE class_id={class_id}'
    result := mysql(query)
    if (result != success)
      return failure
    return success
  else
    return failure
}
bool class_category_name_edit(class_id, category, new_name) // Editing name
{
  if (class_category_exists(class_id, category))
    // Query string should contain properly formatted SQL
    results := class_category_get_all(class_id)
    i = 0
    while (category != grep('/{category}=\d/', results[i]))
      i++
    results[i] := new_name.”=”.value
    categories := implode(“,”, results)
    query := 'UPDATE class SET categories={categories} WHERE class_id={class_id}'
    result := mysql(query)
    if (result != success)
      return failure
    return success
  else
    return failure
}
bool class_category_rank_edit(class_id, category, rank) // Editing position
{
  if (class_category_exists(class_id, category))
    // Query string should contain properly formatted SQL
    results := class_category_get_all(class_id)
    i = 0
    while (category != grep('/{category}=\d/', results[i]))
      i++
    temp := results[i]
    // array_shift pops elements off the front of the array
    j = 0
    while (results)
      if (j = rank)
         new_array[] := temp
      else if (j == i)
        array_shift(results)
      else
        new_array[] := array_shift(results)
      j++    
    categories := implode(“,”, results)
    query := 'UPDATE class SET categories={categories} WHERE class_id={class_id}'
    result := mysql(query)
    if (result != success)
      return failure
    return success
  else
    return failure
}
bool class_category_delete(class_id, category)
{
  if (class_category_exists(class_id, category))
    // Query string should contain properly formatted SQL
    results := class_category_get_all(class_id)
    // The magic function delete_array_element_matching returns an array
    categories := implode(“,”, delete_array_element_matching(category))
    query := 'UPDATE class SET categories={categories} WHERE class_id={class_id}'
    result := mysql(query)
    if (result != success)
      return failure
    return success
  else
    return failure
}
string class_category_get(class_id, category)
{
  if (class_exists(class_id))
    // Query string should contain properly formatted SQL
    query := 'SELECT categories FROM class WHERE class_id={class_id}'
    result := mysql(query) // returns a string
    result := grep('/,*({category}=\d+),*/', result)
    if (result != success)
      return failure
    return result
  else
    return failure
}
array class_category_get_all(class_id)
{
  // Query string should contain properly formatted SQL
  query := 'SELECT categories FROM class WHERE class_id={class_id}'
  results := mysql(query)
  results := explode(“,”, results)
  if (results != success)
    return failure
  else
    return results
}
bool class_category_exists(class_id, category)
{
  result := class_category_get(class_id, category)
  if (result != 0)
    return success
  else
    return failure
}
?>