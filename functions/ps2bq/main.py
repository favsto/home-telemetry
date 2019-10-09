# Copyright 2019 - Fausto Fusaro
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#         http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# libraries
import json
import logging
import os
import pytz

from datetime import datetime

from google.api_core import retry
from google.cloud import bigquery

# parameters
BQ_DATASET = 'home_telemetry'
BQ_TABLE = 'environment'

# functions objects
PROJECT_ID = os.getenv('GCP_PROJECT')
BQ = bigquery.Client()

def health_quality_bigquery(event, context):
    """Background Cloud Function to be triggered by Pub/Sub.
    Args:
         event (dict):  The dictionary with data specific to this type of
         event. The `data` field contains the PubsubMessage message. The
         `attributes` field will contain custom attributes if there are any.
         context (google.cloud.functions.Context): The Cloud Functions event
         metadata. The `event_id` field contains the Pub/Sub message ID. The
         `timestamp` field contains the publish time.
    """
    import base64

    logging.info("""This Function was triggered by messageId {} published at {}
    """.format(context.event_id, context.timestamp))

    registry_id = event['attributes']["deviceRegistryId"]
    device_id = event['attributes']["deviceId"]
    complete_id = "%s.%s" % (registry_id, device_id)
    logging.info('Device info: %s' % complete_id)

    if 'data' in event:
        content = base64.b64decode(event['data']).decode('utf-8')
    else:
        logging.error('%s: No data available in this request' % complete_id)
        return

    logging.info('{}: Received: {}'.format(complete_id, content))

    try:
        record = json.loads(content)
    except:
        logging.info('%s: Received a signal without a JSON: %s' % (complete_id, content))
        return
    
    record["timestamp"] = _now()
    record["registry_id"] = registry_id
    record["device_id"] = device_id

    try:
        check_preconditions(record)
        
    except:
        logging.error('%s: Values above the max threshold' % complete_id)
        return

    table = BQ.dataset(BQ_DATASET).table(BQ_TABLE)

    errors = BQ.insert_rows_json(table=table, json_rows=[record], retry=retry.Retry(deadline=30))

    if errors != []:
        raise BigQueryError(errors)

def _now():
    return datetime.utcnow().replace(tzinfo=pytz.utc).strftime('%Y-%m-%d %H:%M:%S %Z')

def check_preconditions(record):
    if (max(record['temperature'], record['humidity'], record['pressure'], record['altitude']) > 10000.0):
        exc = ValueError()
        exc.strerror = "Values above the max threshold"
        raise exc

class BigQueryError(Exception):
    '''Exception raised whenever a BigQuery error happened''' 

    def __init__(self, errors):
        super().__init__(self._format(errors))
        self.errors = errors

    def _format(self, errors):
        err = []
        for error in errors:
            err.extend(error['errors'])
        return json.dumps(err)