import { EntityPinDefinition } from "./types";

const pinsJsonUrl =
  "https://raw.githubusercontent.com/glacier-modding/glaciermodding.org/refs/heads/main/docs/modding/hitman/guides/pins.json";

export interface PinDefinitionsHelper {
  pinDefinitions: EntityPinDefinition[];
  findPinDefinition(hashOrPath: string): EntityPinDefinition | undefined;
}

export async function loadPinDefinitions(): Promise<PinDefinitionsHelper> {
  const response = await fetch(pinsJsonUrl);
  if (!response.ok) {
    throw new Error(
      `Failed to fetch pin definitions: ${response.status} ${response.statusText}`,
    );
  }

  // TODO: validate this?
  const pinDefinitions = (await response.json()) as EntityPinDefinition[];

  return {
    pinDefinitions,
    findPinDefinition(hashOrPath) {
      return (
        pinDefinitions.find(
          (def) => def.hash === hashOrPath || def.path === hashOrPath,
        ) ?? undefined
      );
    },
  };
}
