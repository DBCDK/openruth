<?php

/**
 * Get list of user loans.
 */
function openruth_loan_list($account) {
  $creds = ding_user_get_creds($account);
  $result = array();
  $status = _openruth_user_status($creds);

  if (isset($status->loans->loan)) {
    foreach ($status->loans->loan as $loan) {
      $result[$loan->copyId] = new DingProviderLoan($loan->copyId, array(
                       'ding_entity_id' => variable_get('openruth_agency_id', '') . ':' . $loan->itemId,
                       'display_name' => $loan->itemDisplayTitle,
                       'loan_date' => $loan->loanDate,
                       'expiry' => $loan->loanReturnDate,
                       'renewable' => $loan->loanRenewable == 'renewable',
                       'materials_number' => $loan->copyId,
                     ));

    }
  }

  return $result;
}

/**
 * Renew loans.
 */
function openruth_loan_renew($account, $ids) {
  $creds = ding_user_get_creds($account);

  $result = array();
  $res = openruth_client_invoke('renew_loan', $creds['name'], $ids);

  if ($res == TRUE) {
    $status = _openruth_user_status($creds, TRUE);
  }
  foreach ($ids as $id) {
    if ($res[$id] === TRUE) {
      $result[$id] = DingProviderLoan::STATUS_RENEWED;
    }
    else {
      $result[$id] = DingProviderLoan::STATUS_NOT_RENEWED;
    }
  }
  return $result;
}
